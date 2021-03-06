#ifndef HASH_BOARD_H_
#define HASH_BOARD_H_

#include <string>

#include "board.h"

namespace Hex {

typedef enum HashColor {HBlack = 1, HWhite = 2};

class HashBoard {
public:
	HashBoard();

	std::string ToAsciiArt();

	void Change(HashColor color, uint position);
	void Change(HashColor color, uint normal_position, bool);
	void Change(HashColor color, uint row, uint column);
	void Change(HashColor color, const std::string &position);
	void Change(const std::string &color, const std::string &position);

	uint Hash(HashColor color, uint position);
	uint Hash(HashColor color, uint normal_position, bool);
	uint Hash(HashColor color, uint row, uint column);
	uint Hash(HashColor color, const std::string &position);
	uint Hash(const std::string &color, const std::string &position);

    const uint* GetAllHashes() const; 
    const bool* GetPositionsPlayed() const; 
    size_t GetBoardSize() const;

    HashColor ParseColor(const std::string &color);
private:
	void InitHash(uint *init, uint position);

	uint _total_size;
	uint _hash_board[kBoardSizeAligned * kBoardSizeAligned];
    bool _played[kBoardSizeAligned * kBoardSizeAligned];
};

} // namespace Hex

#endif
