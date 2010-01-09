#ifndef FAST_SAMPLE_CPP_
#define FAST_SAMPLE_CPP_

#include "fast_sample.h"
#include "board.cpp"
#include "positions.h"

#include <sstream>

namespace Hex {

FastSample::FastSample(float *gammas) {
/* CAUTION!
 * FastSample assumes that FillArrays() and Rand::Init() have already been called
 */
	all_sum = 0.0f;

	memset(_gammas, 0, kBoardSizeAligned * kBoardSizeAligned * sizeof(float));
	memset(_rows, 0, kBoardSizeAligned * sizeof(float));

	if (gammas)
		FillGammas(gammas);
}

void FastSample::FillGammas(float *gammas)
{
	float sum;

	for (uint i = 0; i < kBoardSize; ++i) {
		sum = 0.0f;
		for (uint j = 0; j < kBoardSize; ++j)
			sum += _gammas[(i + 1) * kBoardSizeAligned + 1 + j] = gammas[i * kBoardSize + j];
		_rows[i + 1] = sum;
		all_sum += sum;
	}

	return;
}

std::string FastSample::ToAsciiArt() {
	std::stringstream result;

	for (uint i = 0; i < kBoardSize; ++i) {
		for (uint j = 0; j < kBoardSize; ++j)
			result << _gammas[(i + 1) * kBoardSizeAligned + 1 + j] << " ";
		result << " sum = " << _rows[i + 1] << "\n";
	}

	result << "all_sum = " << all_sum << "\n";

	return result.str();
}

inline void FastSample::Change(uint position, float *new_gammas) {
// position is in 0..(kBoardSize ^ 2)
	return Change(transf[position], new_gammas, true);
}

inline void FastSample::Change(uint normal_position, float *new_gammas, bool) {
// normal_position in 0..(kBoardSizeAligned ^ 2)

	all_sum -= _rows[normal_rows[normal_position] - 1] +
		_rows[normal_rows[normal_position] + 1] +
		_rows[normal_rows[normal_position]];

	_rows[normal_rows[normal_position] - 1] -=
		_gammas[normal_position - kBoardSizeAligned + 1] +
		_gammas[normal_position - kBoardSizeAligned];
	_rows[normal_rows[normal_position] - 1] +=
		(_gammas[normal_position - kBoardSizeAligned + 1] = new_gammas[0]) +
		(_gammas[normal_position - kBoardSizeAligned] = new_gammas[5]);

	_rows[normal_rows[normal_position] + 1] -=
		_gammas[normal_position + kBoardSizeAligned] +
		_gammas[normal_position + kBoardSizeAligned - 1];
	_rows[normal_rows[normal_position] + 1] +=
		(_gammas[normal_position + kBoardSizeAligned ] = new_gammas[2]) +
		(_gammas[normal_position + kBoardSizeAligned - 1] = new_gammas[3]);

	_rows[normal_rows[normal_position]] -=
		_gammas[normal_position + 1] + _gammas[normal_position] +
		_gammas[normal_position - 1];
	_rows[normal_rows[normal_position]] +=
		(_gammas[normal_position + 1] = new_gammas[1]) +
		(_gammas[normal_position] = 0.0f) +
		(_gammas[normal_position - 1] = new_gammas[4]);

	all_sum += _rows[normal_rows[normal_position] - 1] +
		_rows[normal_rows[normal_position] + 1] +
		_rows[normal_rows[normal_position]];

	return;
}

inline void FastSample::Change(uint row, uint column, float *new_gammas) {
//row, column in 0..(kBoardSize -1)
	return Change(transf[row * kBoardSize + column], new_gammas, true);
}

inline void FastSample::Change(const std::string &position, float *new_gammas) {
	return Change(transf[ParsePosition(position)], new_gammas, true);
}

inline uint FastSample::Random() {
	float r = ((float) Rand::next_rand() / ((uint(1) << 31) - 1 - 1)) * all_sum;
	uint i, k, j;

	for (i = 1; i <= kBoardSize; ++i)
		if (r < _rows[i])
			break;
		else
			r -= _rows[i];

	k = i * kBoardSizeAligned;

	for (j = 1; j <= kBoardSize; ++j)
		if (r < _gammas[k + j])
			break;
		else
			r -= _gammas[k + j];

	return (i - 1) * kBoardSize + j - 1;
}

} // namespace Hex

#endif
