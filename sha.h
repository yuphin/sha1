#pragma once
#include <array>
#include <string.h>
#include <bitset>
#include <vector>

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
        for (std::size_t i = 0; i < num_blocks; ++i){
            for (int j = 0; j < 16; ++j){
                chunks[80*i + j]  = (uint8_t)tmp[4*j] << 24 | (uint8_t)tmp[4*j+1] << 16 | (uint8_t)tmp[4*j+2] << 8 | (uint8_t)tmp[4*j+3];
            }
        }

        return chunks;
    }
    void digest(const std::string_view& input){
        auto chunks = divide_chunks(input);

    }

}