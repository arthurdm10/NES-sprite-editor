#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <qdebug.h>
#include <QFileDialog>
#include <QMessagebox>
#include <QInputDialog>

#include <QMouseEvent>


#include <algorithm>
#include <array>
#include <vector>
#include <fstream>
#include <iterator>
#include <chrono>
#include <filesystem>



MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	isARomFile(true),
	ui(new Ui::MainWindow) {

	ui->setupUi(this);



	m_tileView.setGraphicsView(ui->editedTilesView);
	ui->editedTilesView->installEventFilter(&m_tileView);
	ui->editedTilesView->viewport()->installEventFilter(&m_tileView);
	ui->editedTilesView->viewport()->setMouseTracking(true);
	ui->editedTilesView->setScene(&m_tileScene);
	ui->editedTilesView->setMouseTracking(true);
	

	setWindowTitle("NES Sprite Editor");

	colorDemo[0] = ui->palColor0;
	colorDemo[1] = ui->palColor1;
	colorDemo[2] = ui->palColor2;
	colorDemo[3] = ui->palColor3;

	

	for (auto& btn : colorDemo) {
		btn->installEventFilter(this);
	}

	Palette::loadPallete("nes.pal");
	Palette::currentPalette = &Palette::palettes[0];
	Palette::currentColorIndex = 3;

	updatePaletteDemo();


	Tileset::tileItem = new GraphicsTileItem;
	m_tileScene.addItem(Tileset::tileItem);


	ui->editedTilesView->setScene(&m_tileScene);
	ui->editedTilesView->scale(8, 8);
	ui->editedTilesView->centerOn(0, 0);


	selectedColor = colorDemo[3];


	GraphicsColorItem::selectedButton = selectedColor;
	GraphicsColorItem::colorRgbLabel = ui->currentColorRgbLabel;



	auto colorId = Palette::currentPalette->colors[3];
	const auto color = Palette::allColors[colorId];
	constexpr qreal rectSz = 16.0;

	Palette::currentColor		= color.val();
	Palette::currentColorIndex	= 3;

	ui->currentColorRgbLabel->setText(QString::fromStdString(color.toString()));

	colorId = 0x00;
	for (std::size_t y = 0; y < Palette::allColors.size() / 8; y++) {
		for (int x = 0; x < 8; x++) {

			const auto color = Palette::allColors[colorId];
			auto *colorRect = new GraphicsColorItem();

			colorRect->setRect(x * rectSz, y * rectSz, rectSz, rectSz);
			colorRect->setBrush(QBrush(qRgb(color.r, color.g, color.b)));

			colorRect->setData(0xcc, colorId);

			m_paletteScene.addItem(colorRect);

			colorId++;
		}
	}

	ui->paletteView->setScene(&m_paletteScene);
}


MainWindow::~MainWindow() {


	if (Tileset::tilePixmap != nullptr) {
		delete Tileset::tilePixmap;
	}

	delete Tileset::tileItem;

	delete ui;
}


void MainWindow::drawGrid() {

	auto w = Tileset::tilePixmap->width() * 10;
	auto h = Tileset::tilePixmap->height() * 10;

	for (int x = 0; x <= w; x += 80)
		m_tileScene.addLine(x, 0, x, h, QPen(Qt::green))->setScale(0.1);

	for (int y = 0; y <= h; y += 80) {
		m_tileScene.addLine(0, y, w, y, QPen(Qt::green))->setScale(0.1);
	}

}

void MainWindow::getFileData(const QString &filename) {
	std::ifstream file(filename.toStdString(), std::ios::binary);

	if (file.is_open()) {
		file >> std::noskipws;

		Tileset::tileData.assign(std::istream_iterator<byte>(file),
			std::istream_iterator<byte>());

		if (Tileset::tileData.size() == 0) {
			//empty file
			QMessageBox::information(this, "", "This file is empty", QMessageBox::Ok);
			return;
		}

		Tileset::tileColorId.assign(Tileset::tilesetHeight, std::vector<byte>(Tileset::tilesetWidth));
		if (isARomFile) {
			//check if is a valid .NES file
			if (std::string(Tileset::tileData.begin(), Tileset::tileData.begin() + 3) == "NES" && Tileset::tileData[3] == 0x1A) {

				auto prgSize = Tileset::tileData[4];
				auto chrSize = Tileset::tileData[5];

				Bitmap::PixelMatrix pixels(Tileset::tilesetHeight, std::vector<Bitmap::Pixel>(Tileset::tilesetWidth));


				//skip PRG bank (cpu instructions)
				auto dataBegin = Tileset::tileData.begin() + 16 + (0x4000 * prgSize);

				Tileset::parseChrData(dataBegin);
				
				
			} else {
				QMessageBox::information(this, "", "Invalid NES file", QMessageBox::Ok);
			}
		} else {
			//.CHR file
			Tileset::parseChrData(Tileset::tileData.begin());

		}
		setWindowTitle("NES Sprite Editor - " + filename);
		drawGrid();
	}
}



void MainWindow::updatePaletteDemo() {
	for (std::size_t i = 0; i < 0x04; i++) {
		auto colorId = Palette::currentPalette->colors[i];
		auto color = Palette::allColors[colorId];
		auto style = "background-color: rgb(" + std::to_string(color.r) + ", "
			+ std::to_string(color.g) + ", "
			+ std::to_string(color.b) + ");";
		colorDemo[i]->setStyleSheet(QString::fromStdString(style));

		colorDemo[i]->setProperty("colorIndex", i);
		colorDemo[i]->setToolTip(QString::number(colorId, 16).toUpper());
	}
}



void MainWindow::on_actionOpen_triggered() {
	namespace fs = std::filesystem;

	auto filename = QFileDialog::getOpenFileName(this,
		"Open .nes file",
		"D:", "NES ROM (*.nes) ;; NES Sprites (*.chr)");

	QString fileExtension = QString::fromStdString(fs::path(filename.toStdString()).extension().string());

	isARomFile = fileExtension.toLower() == ".nes";

	if (!filename.isNull()) {
		getFileData(filename);
	} 


}


void MainWindow::on_actionNew_Tileset_triggered() {


	if (Tileset::tilePixmap != nullptr) {
		delete Tileset::tilePixmap;
	}

	Tileset::tileColorId.assign(Tileset::tilesetHeight, std::vector<byte>(Tileset::tilesetWidth, 0x00));
	Tileset::tilePixmap = new QPixmap(Tileset::tilesetWidth, Tileset::tilesetHeight);
	Tileset::tileData.assign(Tileset::tilesetWidth * Tileset::tilesetHeight, 0x00);
	Tileset::tileItem->setPixmap(*Tileset::tilePixmap);

	Palette::updateTilesPalette();
	isARomFile = false;
	drawGrid();
}


void MainWindow::on_paletteCombobox_currentIndexChanged(int index) {

	if (index >= 0) {
		Palette::currentPalette = &Palette::palettes[index];
		qDebug() << "Changing palette";
		updatePaletteDemo();
		Palette::updateTilesPalette();
	}

}

void MainWindow::on_newPaletteBtn_clicked() {
	QString paletteName = QInputDialog::getText(this, "Add new palette", "Palette Name:");

	if (!paletteName.isNull()) {
		Palette::palettes.emplace_back();
		ui->paletteCombobox->addItems({ paletteName });

		ui->paletteCombobox->setCurrentIndex(Palette::palettes.size() - 1);
	}
}

void MainWindow::on_delPaletteBtn_clicked() {

	auto index = ui->paletteCombobox->currentIndex();

	if (index >= 0) {
		ui->paletteCombobox->removeItem(index);
	
		Palette::palettes.erase(Palette::palettes.begin() + index);
	}

}



bool MainWindow::eventFilter(QObject* obj, QEvent *event) {
	if (std::find(colorDemo.begin(),
		colorDemo.end(),
		obj) != colorDemo.end()) {

		if (event->type() == QEvent::MouseButtonPress) {
			auto mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->button() == Qt::MouseButton::LeftButton) {

				if (selectedColor != nullptr) {
					selectedColor->setChecked(false);
				}

				auto btn = reinterpret_cast<QPushButton*>(obj);
				selectedColor = btn;
				selectedColor->setChecked(true);

				const auto colorIndex = *(static_cast<byte*>(btn->property("colorIndex").data()));
				const auto colorId = Palette::currentPalette->colors[colorIndex];
				const auto pixelColor = Palette::allColors[colorId];
				const auto colorVal = pixelColor.val();

				Palette::currentColorIndex = colorIndex;
				Palette::currentColor = colorVal;

				GraphicsColorItem::selectedButton = selectedColor;

				ui->currentColorRgbLabel->setText(QString::fromStdString(pixelColor.toString()));

				return true;
			}
		}
	}

	return false;
}

void MainWindow::on_actionSave_ROM_triggered() {
	QString fileType = (isARomFile ? "NES ROM (*.nes)" : "NES Sprites (*.chr)");

	auto filename = QFileDialog::getSaveFileName(this,
		"Save ROM",
		QDir::currentPath(),
		fileType);

	std::ofstream file(filename.toStdString(), std::ios::binary);

	if (file.is_open()) {


		auto begin = Tileset::tileData.begin();
		if (isARomFile) {
			auto prgSize = Tileset::tileData[4];



			//points to tile data
			begin += 16 + (0x4000 * prgSize);
		}

			//plane 1 = bit 0
			//plane 2 = bit 1
			for (uint yRow = 0; yRow < 256; yRow += 8) {

				for (uint nTile = 0; nTile < 16 * 8; nTile += 8) {
					for (uint y = 0; y < 8; y++) {
						byte plane1byte = 0x00;
						byte plane2byte = 0x00;
						byte bitPos = 0x07;


						//get one byte
						for (uint x = 0; x < 8; x++) {
							auto b = Tileset::tileColorId[y + yRow][x + nTile];
							plane1byte |= (b & 0x01) << bitPos;
							plane2byte |= ((b & 0x02) >> 1) << bitPos--;
						}

						*begin = plane1byte;
						*(begin + 8) = plane2byte;

						begin++;
					}
					begin += 8;
				}


			}



			file.write(reinterpret_cast<char*>(Tileset::tileData.data()), Tileset::tileData.size());
		}

	}














