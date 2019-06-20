#pragma once
#include <array>
#include <string.h>
#include <bitset>
#include <vector>

/* Compile time utility functions
 * See https://tools.ietf.org/html/rfc3174#section-7
 */
constexpr uint32_t rotate_left(uint32_t word,uint32_t shift){
    return (word << shift) | (word >> (32-shift));
}
constexpr uint32_t f_1(uint32_t B,uint32_t C,uint32_t D){
    return (B & C) | ((!B) & D);
}
constexpr uint32_t f_2(uint32_t B,uint32_t C,uint32_t D){
    return B ^ C ^ D;
}
constexpr uint32_t f_3(uint32_t B,uint32_t C,uint32_t D){
    return (B & C) | (B & D) | (C & D);
}
namespace sha1{
    std::vector<uint32_t > divide_chunks(const std::string_view& input){
        auto tmp = std::string(input);
        long len_bits = input.length()*8;
        auto num_bytes = ((448 - len_bits ) % 512 + 504) / 8;
        char zeros[num_bytes];
        char length[8];
        memset(zeros,0, sizeof(zeros));
        length[0] = len_bits >> 56; length[1] = len_bits >> 48;
        length[2] = len_bits >> 40; length[3] = len_bits >> 32;
        length[4] = len_bits >> 24; length[5] = len_bits >> 16;
        length[6] = len_bits >> 8;  length[7] = len_bits;
        tmp += 0x80;
        tmp.append(zeros,num_bytes).append(length,8);
        int num_blocks = tmp.length() / 64;
        std::vector<uint32_t > chunks(80*num_blocks,0);
        /*
        for (std::size_t i = 0; i < tmp.size(); ++i){
            std::cout << std::bitset<8>(tmp.c_str()[i]) << std::endl;
        }
        */
        for (std::size_t i = 0; i < num_blocks; ++i){
            for (int j = 0; j < 16; ++j){
                chunks[80*i + j]  = (uint8_t)tmp[4*j] << 24 | (uint8_t)tmp[4*j+1] << 16 | (uint8_t)tmp[4*j+2] << 8 | (uint8_t)tmp[4*j+3];
            }
            for (int j = 16; j < 80; ++j){
                chunks[80*i + j]  = rotate_left(chunks[80*i+ j -3] ^ chunks[80*i+ j -8] ^ chunks[80*i+ j -14] ^ chunks[80*i+ j -16],1);

            }
        }
        /*
        for (std::size_t i = 0; i < 16; ++i){
            std::cout << std::bitset<32>(chunks[i]) << std::endl;
        }
        */
        return chunks;
    }
    void digest(const std::string_view& input){
        auto chunks = divide_chunks(input);

    }

}