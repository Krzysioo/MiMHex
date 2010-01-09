#ifndef FAST_SAMPLE_H_
#define FAST_SAMPLE_H_

#include "board.h"

#include <string>

namespace Hex {

class FastSample {
public:
	FastSample(float *gammas = 0);
	void FillGammas(float *gammas);

	std::string ToAsciiArt();

	void Change(uint position, float *new_gammas);
	void Change(uint normal_position, float *new_gammas, bool);
	void Change(uint row, uint column, float *new_gammas);
	void Change(const std::string &position, float *new_gammas);

	uint Random();

private:
	float _gammas[kBoardSizeAligned * kBoardSizeAligned], _rows[kBoardSizeAligned];
	float all_sum;
};

}
#endif
