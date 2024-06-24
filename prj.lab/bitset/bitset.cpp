#include "bitset.hpp"
#include <stdexcept>
#include <sstream>


BitSet::BitSet(const int32_t size)
        : size_(size)
{
    if (size <= 0) {
        throw std::logic_error("size can't be less than one");
    }
    int k = size / 32;
    if (size%32 != 0) {
        k++;
    }

    for (int i = 0; i < k; i++) {
        data_.push_back(0);
    }
}

BitSet::BitSet(BitSet&& rhs) noexcept {
    std::swap(data_, rhs.data_);
    std::swap(size_, rhs.size_);
}

BitSet& BitSet::operator=(BitSet&& rhs) noexcept {
    if (this != &rhs) {
        std::swap(size_, rhs.size_);
        std::swap(data_, rhs.data_);
    }

    return *this;
}

int32_t BitSet::Size() const noexcept {
    return size_;
}

void BitSet::Resize(const int32_t size) {
    if (size <= 0) {
        throw std::logic_error("size can't be less than one");
    }

    int k = size / 32;
    if (size%32 != 0) {
        k++;
    }

    while (data_.size() != k) {
        if (data_.size() < k) {
            data_.push_back(0);
        } else {
            data_.pop_back();
        }
    }

    if (size < size_) {
        int32_t mask_len = (size)%32;
        uint32_t mask = 0;
        for (int i = 0; i < mask_len; i++) {
            mask += 1 << i;
        }
        int32_t tail = data_.size()-1;
        data_[tail] = data_[tail] & mask;
    }

    size_ = size;
}

void BitSet::fillFromIdxToEnd(int from, bool v) {
    for (from; from < size_; from++) {
        Set(from, v);
    }
}

void BitSet::Fill(const bool v) {
    int k = size_ / 32;
    if (v) {
        for (int i = 0; i < k; i++) {
            data_[i] = ~uint32_t(0);
        }
    } else {
        for (int i = 0; i < k; i++) {
            data_[i] = uint32_t(0);
        }
    }

    if (size_%32 != 0) {
        int from = k == 0 ? 0 : k * 32 -1;
        fillFromIdxToEnd(from, v);
    }



}

void BitSet::Set(const int32_t index, const bool v) {
    if (index >= size_ || index < 0) {
        throw std::out_of_range("set index cant be more than size");
    }

    int32_t block = index/32;

    int32_t index_in_block = index - block*32;
    if (v) {
        uint32_t mask = 1 << index_in_block;
        data_[block] = data_[block] | mask;
    } else {
        uint32_t mask = ~(1 << index_in_block);
        data_[block] = data_[block] & mask;
    }
}

bool BitSet::Get(const int32_t index) const {
    if (index >= size_ || index < 0) {
        throw std::out_of_range("set index cant be more than size");
    }

    int32_t block = index/32;

    int32_t index_in_block = index - block*32;

    uint32_t mask = (1 << index_in_block);

    mask = data_[block] & mask;
    if (mask != 0) {
        return true;
    }
    return false;
}

BitSet& BitSet::operator&=(const BitSet& rhs) {
    if (size_ != rhs.size_) {
        throw std::logic_error("different sizes of bitsets, cant compare");
    }

    for (int i = 0; i < data_.size(); i++) {
        data_[i] &= rhs.data_[i];
    }

    return *this;
}

BitSet& BitSet::operator|=(const BitSet& rhs) {
    if (size_ != rhs.size_) {
        throw std::logic_error("different sizes of bitsets, cant compare");
    }

    for (int i = 0; i < data_.size(); i++) {
        data_[i] |= rhs.data_[i];
    }

    return *this;
}

BitSet& BitSet::operator^=(const BitSet& rhs) {
    if (size_ != rhs.size_) {
        throw std::logic_error("different sizes of bitsets, cant compare");
    }

    for (int i = 0; i < data_.size(); i++) {
        data_[i] ^= rhs.data_[i];
    }

    return *this;
}

BitSet operator&(const BitSet& lhs, const BitSet& rhs) {
    if (lhs.Size() != rhs.Size()) {
        throw std::logic_error("cant compare two bitsets with different size");
    }

    BitSet res = lhs;

    res &= rhs;

    return res;
}

BitSet operator|(const BitSet& lhs, const BitSet& rhs) {
    if (lhs.Size() != rhs.Size()) {
        throw std::logic_error("cant compare two bitsets with different size");
    }

    BitSet res = lhs;

    res |= rhs;

    return res;
}

BitSet operator^(const BitSet& lhs, const BitSet& rhs) {
    if (lhs.Size() != rhs.Size()) {
        throw std::logic_error("cant compare two bitsets with different size");
    }

    BitSet res = lhs;

    res ^= rhs;

    return res;
}

bool BitSet::operator==(const BitSet& rhs) const noexcept {
    if (size_ != rhs.size_) {
        return false;
    }

    for (int i = 0; i < rhs.data_.size(); i++) {
        if (data_[i] != rhs.data_[i]) {
            return false;
        }
    }


    return true;
}

bool BitSet::operator!=(const BitSet& rhs) const noexcept {
    return !(*this==rhs);
}

BitSet BitSet::operator~() {
    BitSet res(*this);

    for (int i = 0; i < size_; i++) {
        res.Set(i, !res.Get(i));
    }
    return res;
}

bool BitSet::operator[](const int32_t lhs) const {
    return bool(this->Get(lhs));
}

BitSet::BitAccessor BitSet::operator[](const int32_t rhs) {
    if (!(rhs >= 0 && rhs < size_)) {
        throw std::out_of_range("index out of range");
    }

    return {*this, rhs};
}

std::ostream& BitSet::writeBinaryData(std::ostream& rhs) const noexcept {
    uint8_t window_base = ~uint8_t(0);

    for (int i = 0; i < data_.size(); i++) {
        for (int k = 0; k < 4; k++) {
            uint32_t window = window_base << (8 * k);
            auto boba = uint8_t((data_[i] & window) >> (8 * k));
            rhs << boba;
        }
    }

    return rhs;
}

uint8_t isEven(const std::vector<uint32_t>& rhs) {
    uint8_t count = 0;
    for (uint32_t i : rhs) {
        for (int k = 0; k < 31; k++) {
            uint32_t bit = (i & (1 << k));
            if (bit > 0) {
                count++;
            }
        }
    }

    uint8_t even_mark = uint8_t(0);
    if (count%2 != 0) {
        even_mark = ~even_mark;
    }

    return even_mark;
}

std::ostream& BitSet::WriteBinary(std::ostream& rhs) const noexcept {
    uint8_t even_mark = isEven(data_);

    rhs << start_mark_;

    for (int i = 3; i >= 0; i--) {
        uint8_t d = ((size_ >> (8*i)));
        rhs << d;
    }

    writeBinaryData(rhs);

    rhs << even_mark << end_mark_;

    return rhs;
};

std::istream& BitSet::ReadBinary(std::istream& rhs) {
    char start_mark = 0;
    uint32_t size = 0;
    uint8_t even_mark = 0;
    char end_mark = 0;

    rhs >> start_mark;

    for (int i = 3; i >= 0; i--) {
        uint8_t size_fragment = 0;
        rhs >> size_fragment;
        size |= (size_fragment << i*8);
    }

    if (start_mark != start_mark_) {
        throw std::logic_error("start mark isnt correct");
    }

    if (size <= 0) {
        throw std::logic_error("size cant be less than 1");
    }

    Fill(false);

    uint32_t length = size/32;
    if (size%32 != 0) {
        length++;
    }
    uint32_t current_length = size_/32;
    if (size_%32 != 0) {
        current_length++;
    }

    if (current_length > length) {
        while (current_length != length) {
            data_.pop_back();
            current_length--;
        }
    } else if (current_length < length) {
        while (current_length != length) {
            data_.push_back(0);
            current_length++;
        }
    }

    size_ = size;

    uint8_t block = 0;
    for (int i = 0; i < data_.size(); i++) {
        for (int k = 0; k < 4; k++) {
            rhs >> block;
            uint32_t mask = block << (8 * k);
            data_[i] |= mask;
        }
    }

    rhs >> even_mark >> end_mark;
    auto d = isEven(data_);
    if (isEven(data_) != even_mark) {
        throw std::logic_error("even_mark doesnt match eveness");
    }
    if (end_mark != end_mark_) {
        throw std::logic_error("end mark isnt correct");
    }

    return rhs;
}

std::ostream& BitSet::WriteTxt(std::ostream& rhs) const noexcept {
    uint8_t mask_base = uint8_t(1);
    std::string space;

    for (int i = 0; i < data_.size(); i++) {
        for (int k = 0; k < 4; k++) {
            uint32_t mask = (mask_base << k*8);
            for (int d = 0; d < 8; d++) {
                uint32_t cur_elem = (data_[i] & (mask << d));
                uint32_t cur_pos = (32 * i) + (k * 8) + d;
                if (cur_pos < size_) {
                    if (cur_elem > 0) {
                        rhs << 1;
                    } else {
                        rhs << 0;
                    }
//                }
                } else {
                    space += " ";
                }
            }
            rhs << " ";
        }
        rhs << space;
        rhs << "| " << i + 1 << std::endl;
    }

    rhs << "=" << std::endl;

    return rhs;
}

std::istream& BitSet::ReadTxt(std::istream& rhs) {

    std::vector<std::string> buffer;

    for (std::string line; std::getline(rhs, line);) {
        buffer.push_back(line);
    }

    Resize((buffer.size() - 1) * 32);

    int index = -1;
    for (std::string line: buffer) {
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '0') {
                index++;
                Set(index, false);
            } else if (line[i] == '1') {
                index++;
                Set(index, true);
            } else if (line[i] == '|') {
                break;
            }
        }
    }

    Resize(index + 1);

    return rhs;
}