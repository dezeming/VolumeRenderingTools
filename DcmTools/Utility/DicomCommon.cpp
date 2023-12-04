/*
	Copyright (C) <2023>  <Dezeming>  <feimos@mail.ustc.edu.cn>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or any
	later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.

	Github site: <https://github.com/dezeming/VolumeRenderingTools.git>
*/

#include "DicomCommon.hpp"

#include <limits>


bool VolumeData::getVolumeStatistics() {

	if (!data) return false;
	if (format != Dez_Float && format != Dez_Double) return false;

	statistics.data = static_cast<float *>(data);

	const float float_max = std::numeric_limits<float>::max();
	const float float_min = std::numeric_limits<float>::min();

	statistics.max = float_min;
	statistics.min = float_max;

	for (unsigned int k = 0; k < zResolution; k++) {
		for (unsigned int i = 0; i < xResolution; i++) {
			for (unsigned int j = 0; j < yResolution; j++) {
				size_t offset = k * xResolution * yResolution + i + j * xResolution;
				if (statistics.min > statistics.data[offset])
					statistics.min = statistics.data[offset];
				if (statistics.max < statistics.data[offset])
					statistics.max = statistics.data[offset];
			}
		}
	}

	statistics.gradientMax = float_min;
	statistics.gradientMin = float_max;
	size_t imagePixels = xResolution * yResolution;
	for (unsigned int k = 0; k < zResolution; k++) {
		for (unsigned int i = 0; i < xResolution; i++) {
			for (unsigned int j = 0; j < yResolution; j++) {

				unsigned int k0 = (k == 0 ? 0 : k - 1), k2 = (k == zResolution - 1 ? k : k + 1);
				unsigned int i0 = (i == 0 ? 0 : i - 1), i2 = (i == xResolution - 1 ? i : i + 1);
				unsigned int j0 = (j == 0 ? 0 : j - 1), j2 = (j == yResolution - 1 ? j : j + 1);

				size_t offset_1_1_1 = k * imagePixels + i + j * xResolution;

				size_t offset_1_1_2 = k2 * imagePixels + i + j * xResolution;
				size_t offset_1_1_0 = k0 * imagePixels + i + j * xResolution;
				size_t offset_1_2_1 = k * imagePixels + i + j2 * xResolution;
				size_t offset_1_0_1 = k * imagePixels + i + j0 * xResolution;
				size_t offset_2_1_1 = k * imagePixels + i2 + j * xResolution;
				size_t offset_0_1_1 = k * imagePixels + i0 + j * xResolution;
				
				float xx = statistics.data[offset_2_1_1] - statistics.data[offset_0_1_1];
				float yy = statistics.data[offset_2_1_1] - statistics.data[offset_0_1_1];
				float zz = statistics.data[offset_2_1_1] - statistics.data[offset_0_1_1];

				float gradientMagnitudeSquare = xx * xx + yy * yy + zz * zz;

				if (statistics.gradientMin > gradientMagnitudeSquare)
					statistics.gradientMin = gradientMagnitudeSquare;
				if (statistics.gradientMax < gradientMagnitudeSquare)
					statistics.gradientMax = gradientMagnitudeSquare;
			}
		}
	}

	statistics.gradientMin = std::sqrt(statistics.gradientMin);
	statistics.gradientMax = std::sqrt(statistics.gradientMax);

	return true;
}



