#pragma once

#define _HAS_STD_BYTE 0

#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <sstream>


using byte = unsigned char;
using WORD = unsigned short;
using DWORD = unsigned int;
using UINT = unsigned int;


class Bitmap {
public:
	struct Pixel {
        byte b, g, r;

        Pixel() = default;

        Pixel(byte r, byte g, byte b) :
            r(r), g(g), b(b) {}

		Pixel(Pixel&& p) noexcept :
            b(std::exchange(p.b, 0x00)),
			g(std::exchange(p.g, 0x00)),
            r(std::exchange(p.r, 0x00)) {}

		Pixel(const Pixel& other)
            :b(other.b), g(other.g), r(other.r) {}

        UINT val()const {
            return (0xffu << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu);
        }


		Pixel& operator=(const Pixel& rhs) {
			if (this != &rhs) {
				this->b = rhs.b;
				this->g = rhs.g;
				this->r = rhs.r;
			}
			return *this;
		}

        bool operator==(const Pixel& rhs)  {
			return (this->b == rhs.b &&
					this->g == rhs.g &&
					this->r == rhs.r);
		}

        bool operator!=(const Pixel& rhs)  { return !(*this == rhs); }

        std::string toString() const {
            std::stringstream ss;
            ss << "RGB(" << static_cast<int>(r) << ", "
                           << static_cast<int>(g) << ", "
                           << static_cast<int>(b) << ")";
            return ss.str();
        }

	};


private:
	template<class T>
    inline void readFromStream(std::ifstream& file, T& dest, std::size_t  sz = sizeof(T)) {
		file.read(reinterpret_cast<char*>(&dest), sz);
	}

	template<class T>
    inline void writeToStream(std::ofstream& file, const T& src, std::size_t  sz = sizeof(T)) {
		file.write(reinterpret_cast<const char*>(&src), sz);
	}


    template<class T>
    inline void pushData(std::vector<byte>* vec, T data, std::size_t dataSize = sizeof(T)) const{

        for(int i = 0; i < dataSize; ++i){
            vec->push_back((data >> i * 8) & 0xff);
        }
    }

    template<>
    inline void pushData<byte>(std::vector<byte>* vec, byte data, std::size_t dataSize) const {

        vec->push_back(data);
    }

    template<>
    inline void pushData<const Bitmap::Pixel*>(std::vector<byte>* vec,  const Bitmap::Pixel* data, std::size_t dataSize) const {
        for(int i = 0; i < dataSize; ++i){
            auto[b, g, r] = *data++;
            vec->insert(vec->end(), {b, g, r});
         }
    }

    template<>
    inline void pushData<byte*>(std::vector<byte>* vec, byte* data, std::size_t dataSize) const {
        for(int i = 0; i < dataSize; ++i){
            vec->push_back(*data++);
        }
    }

public:
	typedef std::vector<std::vector<Bitmap::Pixel>> PixelMatrix;

private:
	std::vector<Pixel> pixels;

	void readBitmapHeader(std::ifstream& file);
	void readDibHeader(std::ifstream& file);
    size_t rowSz;                           //size of a row in bytes
	
	DWORD m_fileSize,
		m_width,
		m_height;

public:
	Bitmap() = default;
	Bitmap(Bitmap&& bitmap) noexcept;
	Bitmap(const Bitmap& bitmap) noexcept;
	Bitmap(const std::string& fileName);
	Bitmap(const PixelMatrix& pixels);
	Bitmap(PixelMatrix&& pixels);

	bool load(const std::string& fileName);

	//create a bitmap from a matrix of pixels
	bool load(const PixelMatrix& pixels);
	bool save(const std::string& fileName);


    std::vector<byte> data() const;


	Bitmap roi(DWORD x, DWORD y, DWORD width, DWORD height);

	void toGrayScale();
	
	void mirror();
	void horizontalMirror() { flip180(); mirror(); }
	void flip180() { std::reverse(pixels.begin(), pixels.end()); }
	void flipLeft();
	void flipRight();

	Pixel& operator()(DWORD x, DWORD y);

    UINT fileSize()	const noexcept { return m_fileSize; }
    UINT dataSize()	const noexcept { return m_width * m_height * 3; }
	DWORD width()			const noexcept { return m_width; }
	DWORD height()			const noexcept { return m_height; }

	Bitmap& operator=(const Bitmap& other);    //copy
	Bitmap& operator=(Bitmap&& other);          //move
	bool operator== (const Bitmap& other);

	operator bool() const { return this->pixels.size() > 0; }
	bool operator!() const { return !(*this); }

	~Bitmap() = default;
};

std::ostream& operator<< (std::ostream& out, const Bitmap::Pixel& pixel);
