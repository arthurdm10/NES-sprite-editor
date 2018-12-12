#include "Bitmap.h"
#include <iostream>
#include <algorithm>
#include <exception>

using namespace std;


Bitmap::Bitmap(Bitmap &&bitmap) noexcept
    :pixels(std::move(bitmap.pixels)),
      rowSz(std::exchange(bitmap.rowSz, 0x00)),
      m_fileSize(std::exchange(bitmap.m_fileSize, 0x00)),
      m_width(std::exchange(bitmap.m_width, 0x00)),
      m_height(std::exchange(bitmap.m_height, 0x00)){}

Bitmap::Bitmap(const Bitmap &bitmap) noexcept
	:m_width(bitmap.m_width),
	m_height(bitmap.m_height),
	m_fileSize(bitmap.m_fileSize),
	pixels(bitmap.pixels),
	rowSz(bitmap.rowSz){}

Bitmap::Bitmap(const std::string& fileName) {
	this->load(fileName);
}

Bitmap::Bitmap(const Bitmap::PixelMatrix &pixels) {
	this->load(pixels);
}

Bitmap::Bitmap(Bitmap::PixelMatrix&& pixels) {
	this->load(std::forward<PixelMatrix>(pixels));
}

bool Bitmap::load(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary);
	if (file.is_open()) {
		this->readBitmapHeader(file);
		this->readDibHeader(file);
	} else {
		return false;
	}

	if (m_width == 0 || m_height == 0) {
		return false;
	}


	this->rowSz = 3 * m_width;         //1 pixel = 3 bytes


	file.seekg(54, file.beg);
	auto sz = m_width * m_height;
	this->pixels.resize(sz);

	for (DWORD i = 0; i < m_height; ++i) {
		auto pos = sz - (i * m_width) - m_width;
		file.read(reinterpret_cast<char*>(&this->pixels[pos]), this->rowSz);
		file.seekg(m_width % 4, file.cur);
	}
    m_fileSize = 54 + (this->pixels.size() * 3) + (m_width % 4) * this->pixels.size();

	return true;
}

bool Bitmap::load(const PixelMatrix& pixels) {

	if (pixels.empty() || pixels[0].empty()) {
		return false;
	}

	//check if all rows have the same size
	auto sz = pixels[0].size();
	auto p2 = std::any_of(std::begin(pixels),
		std::end(pixels),
		[&sz](const auto& row) {
		return row.size() != sz;
	});

	if (p2) {
		//found one
		throw std::runtime_error("All rows must have the same size");
	}

	m_width = sz;
    m_height = pixels.size();
	this->rowSz = 3 * m_width;


	this->pixels.reserve(m_width * m_height);

	std::for_each(std::begin(pixels),
		std::end(pixels),
		[this](auto& row) {
        this->pixels.insert(this->pixels.end(), row.cbegin(), row.cend());
	});


	m_fileSize = 54 + (this->pixels.size() * 3) + (m_width % 4) * this->pixels.size();

	return true;
}

bool Bitmap::save(const std::string& fileName) {
	if (this->pixels.size() == 0) {
		return false;
	}

	std::ofstream output(fileName, std::ios::binary);

	if (output.is_open()) {

		//bitmapHeader
		writeToStream(output, 0x4d42, 2);
		writeToStream(output, m_fileSize, 4);
		writeToStream(output, 0, 2);
		writeToStream(output, 0, 2);
		writeToStream(output, 54, 4);

		//dibHeader
		writeToStream(output, 40, 4);
		writeToStream(output, m_width, 4);
		writeToStream(output, m_height, 4);
		writeToStream(output, 1, 2);
		writeToStream(output, 24, 2);
		writeToStream(output, 0, 4);
		writeToStream(output, 0, 4);
		writeToStream(output, 0, 4);
		writeToStream(output, 0, 4);
		writeToStream(output, 0, 4);
		writeToStream(output, 0, 4);

		//row padding
		WORD paddSz = m_width % 4;
		char *padd = new char[paddSz];
		memset(padd, 0x00, paddSz);
		
		const auto sz = this->pixels.size();
		const auto height = m_height;
		const auto data = this->pixels.data();

		for (DWORD i = 0; i < height; i++) {
			int pos = sz - (i * m_width) - m_width;
			output.write(reinterpret_cast<char*>((data + pos)), this->rowSz);
			output.write(padd, paddSz);
		}

		output.close();
		delete[] padd;
		return true;
	}
    return false;
}

std::vector<byte> Bitmap::data() const {
    std::vector<byte> output;


    //bitmapHeader
    pushData(&output, 0x4d42, 2);
    pushData(&output, m_fileSize, 4);
    pushData(&output, 0, 2);
    pushData(&output, 0, 2);
    pushData(&output, 54, 4);

    //dibHeader
    pushData(&output, 40, 4);
    pushData(&output, m_width, 4);
    pushData(&output, m_height, 4);
    pushData(&output, 1, 2);
    pushData(&output, 24, 2);
    pushData(&output, 0, 4);
    pushData(&output, 0, 4);
    pushData(&output, 0, 4);
    pushData(&output, 0, 4);
    pushData(&output, 0, 4);
    pushData(&output, 0, 4);

    //row padding
    WORD paddSz = m_width % 4;
    byte *padd = new byte[paddSz];
    memset(padd, 0x00, paddSz);

    const auto sz = this->pixels.size();
    const auto height = m_height;
    auto data = this->pixels.data();

    for (DWORD i = 0; i < height; i++) {
        int pos = sz - (i * m_width) - m_width;
        pushData(&output, (data + pos), m_width);
        pushData(&output, padd, paddSz);
    }


    delete[] padd;

    return output;
}

Bitmap Bitmap::roi(DWORD x, DWORD y, DWORD width, DWORD height) {
	
	if (x + width - 1 < m_width && y + height - 1 < m_height) {
		PixelMatrix pixels(height, std::vector<Pixel>(width));

	
		auto _y = y;
		Pixel* rowData;
		const auto data = this->pixels.data();

		for(auto& row : pixels){
			auto pxPos = _y * m_width;

			rowData = row.data();
			for (DWORD _x = x; _x < width + x ; ++_x) {
				*rowData = *(data + (pxPos + _x));
				++rowData;
			}
			++_y;
		}

		return Bitmap(std::move(pixels));
	}

}

void Bitmap::mirror() {
	const auto itBeg = this->pixels.begin();
	
	for (DWORD i = 0; i < m_height; i++) {
		auto row = i * m_width;
		auto it = itBeg + row;
		std::reverse(it, it + m_width);
	}
}

void Bitmap::toGrayScale() {
	float value = 0;
	for (auto& pixel : this->pixels) {
		value = 0.21f * pixel.r + 0.72f * pixel.g + 0.07f * pixel.b;
		pixel.r = pixel.b = pixel.g = static_cast<byte>(value);
	}
}

void Bitmap::flipLeft() {
	std::vector<Pixel> copy(std::move(this->pixels));

	this->pixels.resize(copy.size());
	const auto data = copy.data();

	auto thisData = this->pixels.data();

	for (DWORD x = 0; x < m_width; ++x) {
		for (DWORD y = 0; y < m_height; ++y) {
			/*same as this->pixels[i++] = data[y * m_width + x]*/
			*thisData = *(data + (y * m_width + x));
			++thisData;
		}
	}
	std::swap(m_width, m_height);
	this->rowSz = m_width * 3;
}

void Bitmap::flipRight() {
	flipLeft();
	mirror();
}


Bitmap::Pixel& Bitmap::operator()(DWORD x, DWORD y) {
	if (x > m_width || y > m_height) {
		char msg[255];
		snprintf(msg, 254, "Bitmap(%d, %d) -- Coord X:%d Y:%d out of range. Width is %d and Height is %d",
			x, y,
			x, y,
			m_width, m_height);
		throw std::out_of_range(msg);
	}

	return this->pixels[y * m_width + x];
}

void Bitmap::readBitmapHeader(std::ifstream& file) {
	WORD header;
	readFromStream(file, header);

	if (header != 0x4D42) {
		throw std::exception("Invalid bitmap file");
	}

	readFromStream(file, m_fileSize);
	file.seekg(12, file.cur);
}

void Bitmap::readDibHeader(std::ifstream& file) {
	readFromStream(file, m_width);
	readFromStream(file, m_height);
	file.seekg(2, file.cur);

	WORD bitsPixel = 0;
	readFromStream(file, bitsPixel);

	if (bitsPixel != 24) {
		throw std::exception("Bitmap must be 24bits per pixel");
	}

	DWORD compression;

	readFromStream(file, compression);

	if (compression != 0x0000) {
		throw std::exception("Bitmap can not be compressed");
	}
}

Bitmap& Bitmap::operator=(const Bitmap &rhs) {
	if (this != &rhs) {
		this->pixels = rhs.pixels;
		m_width = rhs.m_width;
		m_height = rhs.m_height;
		m_fileSize = rhs.m_fileSize;
	}
	return *this;
}

Bitmap& Bitmap::operator=(Bitmap&& rhs) {
	if (this != &rhs) {
		this->pixels = std::move(rhs.pixels);
		m_width	   = std::exchange(rhs.m_width, 0x00);
		m_height   = std::exchange(rhs.m_height, 0x00);
		m_fileSize = std::exchange(rhs.m_fileSize, 0x00);
	}
	return *this;
}

bool Bitmap::operator==(const Bitmap& rhs) {
	if (this->pixels.size() != rhs.pixels.size()) {
		return false;
	}

	return (this->pixels == rhs.pixels);
}


std::ostream& operator<<(std::ostream & out, const Bitmap::Pixel & pixel) {
	auto[b, g, r] = pixel;
	out << "(" << static_cast<int>(r) << ", " << static_cast<int>(g) << ", " << static_cast<int>(b) << ")";
	return out;
}


bool operator== (const Bitmap::Pixel& p1, const Bitmap::Pixel& p2) {
	return (p1.b == p2.b &&
		p1.g == p2.g &&
		p1.r == p2.r);
}

bool operator!= (const Bitmap::Pixel& p1, const Bitmap::Pixel& other) {
	return !(p1 == other);
}
