#pragma once

#include <array>
#include <string.h>
#include <vector>
#include <sstream>

/* Utility functions
 * See https://tools.ietf.org/html/rfc3174#section-7
 */
uint32_t rotate_left(uint32_t word, uint32_t shift) {
    return (word << shift) | (word >> (32 - shift));
}

uint32_t f_1(uint32_t B, uint32_t C, uint32_t D) {
    return ((B & C) | ((~B) & D));
}

uint32_t f_2(uint32_t B, uint32_t C, uint32_t D) {
    return (B ^ C ^ D);
}

uint32_t f_3(uint32_t B, uint32_t C, uint32_t D) {
    return ((B & C) | (B & D) | (C & D));
}

namespace sha1 {
    std::vector<uint32_t> divide_chunks(const std::string_view &input, const uint64_t &len_bits) {
        auto tmp = std::string(input);
        uint32_t num_bytes = ((440 -len_bits) % 512 + 512 ) % 512  / 8;
        char zeros[num_bytes];
        char length[8];
        memset(zeros, 0, sizeof(zeros));
        length[0] = len_bits >> 56;
        length[1] = len_bits >> 48;
        length[2] = len_bits >> 40;
        length[3] = len_bits >> 32;
        length[4] = len_bits >> 24;
        length[5] = len_bits >> 16;
        length[6] = len_bits >> 8;
        length[7] = len_bits;
        tmp += (uint8_t)0x80;
        tmp.append(zeros, num_bytes).append(length, 8);
        int num_blocks = tmp.length() / 64;
        std::vector<uint32_t> chunks(80 * num_blocks, 0);
        for (std::size_t i = 0; i < num_blocks; ++i) {
            for (int j = 0; j < 16; ++j) {
                chunks[80 * i + j] =
                        (uint8_t) tmp[64*i + 4 * j] << 24 | (uint8_t) tmp[64*i+ 4 * j + 1] << 16 | (uint8_t) tmp[64*i + 4 * j + 2] << 8 |
                        (uint8_t) tmp[64*i+ 4 * j + 3];
            }
            for (int j = 16; j < 80; ++j) {
                chunks[80 * i + j] = rotate_left(
                        chunks[80 * i + j - 3] ^ chunks[80 * i + j - 8] ^ chunks[80 * i + j - 14] ^
                        chunks[80 * i + j - 16], 1);

            }
        }
        return chunks;
    }

    std::string digest(const std::string_view &input) {
        uint64_t len_bits = input.length() * 8;
        auto chunks = divide_chunks(input, len_bits);
        auto num_chunks = chunks.size() / 80;
        std::vector<uint32_t> digest_vec(5,0);
        uint32_t hs[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
        for (uint32_t i = 0; i < num_chunks; i++) {
            uint32_t A = hs[0];
            uint32_t B = hs[1];
            uint32_t C = hs[2];
            uint32_t D = hs[3];
            uint32_t E = hs[4];
            for (int j = 0; j < 80; j++) {
                uint32_t K, F, tmp;
                if (j < 20) {
                    K = 0x5A827999;
                    F = f_1(B, C, D);
                } else if (j < 40) {
                    K = 0x6ED9EBA1;
                    F = f_2(B, C, D);
                } else if (j < 60) {
                    K = 0x8F1BBCDC;
                    F = f_3(B, C, D);
                } else {
                    K = 0xCA62C1D6;
                    F = f_2(B, C, D);
                }
                tmp = rotate_left(A,5) + F + E + K + chunks[80*i + j];
                E = D;
                D = C;
                C = rotate_left(B,30);
                B = A;
                A = tmp;
            }
            hs[0] += A;
            hs[1] += B;
            hs[2] += C;
            hs[3] += D;
            hs[4] += E;
        }
        std::string result;
        for(uint32_t h : hs){
            std::stringstream stream;
            stream << std::hex << h;
            auto str = stream.str();
            if(str.length() % 8) {
                str = '0' + str;
            }
            result += str;
        }
        return result;
    }

}
