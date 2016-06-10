#include "Angel.h"
#include <fstream>
#include <assert.h>

using namespace std;

class Mesh_viewer {

private :
	ifstream input;
	ofstream output;
	char off[10];
	int none;
	typedef struct VECTOR {
		float x;
		float y;
		float z;
	};
public :
	float* vCoords;
	unsigned int* iArray;
	float* fNorms, *vsNormal, *vfNormal;
	float cX, cY, cZ;
	int iSize, vSize;
	float min_v[3];
	float max_v[3];
	float mid[3];
	Mesh_viewer() {
		none = 0;
		cX = 0;
		cY = 0;
		cZ = 0;
		iSize = 0;
		vSize = 0;
	}
	Mesh_viewer(const char* file) {
		none = 0;
		cX = 0;
		cY = 0;
		cZ = 0;
		iSize = 0;
		vSize = 0;

		input.open(file);

		if (input.is_open()) {
			input >> off;
			if (strcmp(off, "OFF") == 0) {
				input >> vSize >> iSize >> none;
				vCoords = new float[vSize * 3];
				iArray = new unsigned int[iSize * 3];
				vsNormal = new float[vSize * 3]();
				vfNormal = new float[vSize * 3];
				for (int i = 0; i < vSize * 3; i++) {
					input >> vCoords[i];
					//if(vCoords[i*3])
				}
				//cout << "COORDS =  " << vCoords[0] << " " << vCoords[vSize * 3 - 1] << endl;
				int cnt = 0;
				for (int i = 0; i < iSize; i++) {
					int temp;
					input >> temp;
					input >> iArray[cnt] >> iArray[cnt + 1] >> iArray[cnt + 2];
					cnt += 3;

					float u[3] = { vCoords[iArray[cnt - 2] * 3] - vCoords[iArray[cnt - 3] * 3],
						vCoords[iArray[cnt - 2] * 3 + 1] - vCoords[iArray[cnt - 3] * 3 + 1],
						vCoords[iArray[cnt - 2] * 3 + 2] - vCoords[iArray[cnt - 3] * 3 + 2] };

					float v[3] = { vCoords[iArray[cnt - 1] * 3] - vCoords[iArray[cnt - 3] * 3],
						vCoords[iArray[cnt - 1] * 3 + 1] - vCoords[iArray[cnt - 3] * 3 + 1],
						vCoords[iArray[cnt - 1] * 3 + 2] - vCoords[iArray[cnt - 3] * 3 + 2] };


					vsNormal[iArray[cnt - 3] * 3] += -(u[1] * v[2] - u[2] * v[1]);
					vsNormal[iArray[cnt - 3] * 3 + 1] += -(u[2] * v[0] - u[0] * v[2]);
					vsNormal[iArray[cnt - 3] * 3 + 2] += -(u[0] * v[1] - u[1] * v[0]);

					vsNormal[iArray[cnt - 2] * 3] += -(u[1] * v[2] - u[2] * v[1]);
					vsNormal[iArray[cnt - 2] * 3 + 1] += -(u[2] * v[0] - u[0] * v[2]);
					vsNormal[iArray[cnt - 2] * 3 + 2] += -(u[0] * v[1] - u[1] * v[0]);

					vsNormal[iArray[cnt - 1] * 3] += -(u[1] * v[2] - u[2] * v[1]);
					vsNormal[iArray[cnt - 1] * 3 + 1] += -(u[2] * v[0] - u[0] * v[2]);
					vsNormal[iArray[cnt - 1] * 3 + 2] += -(u[0] * v[1] - u[1] * v[0]);
				}
				for (int i = 0; i < vSize * 3; i = i + 3)
				{
					float Eucl = sqrt(vsNormal[i] * vsNormal[i] + vsNormal[i + 1] * vsNormal[i + 1] + vsNormal[i + 2] * vsNormal[i + 2]);

					vsNormal[i] /= Eucl;
					vsNormal[i + 1] /= Eucl;
					vsNormal[i + 2] /= Eucl;
				}
			}
			input.close();
		//	cout << vsNormal[0] << vsNormal[1] << vsNormal[2] << endl;

		}
	}
	void init(const char* file){
		input.open(file);

		if (input.is_open()) {
			input >> off;
			if (strcmp(off, "OFF") == 0) {
				input >> vSize >> iSize >> none;
				vCoords = new float[vSize * 3];
				iArray = new unsigned int[iSize * 3];
				vsNormal = new float[vSize * 3]();
				vfNormal = new float[vSize * 3];
				for (int i = 0; i < vSize * 3; i++) {
					input >> vCoords[i];
				}
				//cout << "COORDS =  " << vCoords[0] << " " << vCoords[vSize * 3 - 1] << endl;
				int cnt = 0;
				for (int i = 0; i < iSize; i++) {
					int temp;
					input >> temp;
					input >> iArray[cnt] >> iArray[cnt + 1] >> iArray[cnt + 2];
					cnt += 3;

					float u[3] = { vCoords[iArray[cnt - 2] * 3] - vCoords[iArray[cnt - 3] * 3],
						vCoords[iArray[cnt - 2] * 3 + 1] - vCoords[iArray[cnt - 3] * 3 + 1],
						vCoords[iArray[cnt - 2] * 3 + 2] - vCoords[iArray[cnt - 3] * 3 + 2] };

					float v[3] = { vCoords[iArray[cnt - 1] * 3] - vCoords[iArray[cnt - 3] * 3],
						vCoords[iArray[cnt - 1] * 3 + 1] - vCoords[iArray[cnt - 3] * 3 + 1],
						vCoords[iArray[cnt - 1] * 3 + 2] - vCoords[iArray[cnt - 3] * 3 + 2] };


					vsNormal[iArray[cnt - 3] * 3] += -(u[1] * v[2] - u[2] * v[1]);
					vsNormal[iArray[cnt - 3] * 3 + 1] += -(u[2] * v[0] - u[0] * v[2]);
					vsNormal[iArray[cnt - 3] * 3 + 2] += -(u[0] * v[1] - u[1] * v[0]);

					vsNormal[iArray[cnt - 2] * 3] += -(u[1] * v[2] - u[2] * v[1]);
					vsNormal[iArray[cnt - 2] * 3 + 1] += -(u[2] * v[0] - u[0] * v[2]);
					vsNormal[iArray[cnt - 2] * 3 + 2] += -(u[0] * v[1] - u[1] * v[0]);

					vsNormal[iArray[cnt - 1] * 3] += -(u[1] * v[2] - u[2] * v[1]);
					vsNormal[iArray[cnt - 1] * 3 + 1] += -(u[2] * v[0] - u[0] * v[2]);
					vsNormal[iArray[cnt - 1] * 3 + 2] += -(u[0] * v[1] - u[1] * v[0]);
				}
				for (int i = 0; i < vSize * 3; i = i + 3)
				{
					float Eucl = sqrt(vsNormal[i] * vsNormal[i] + vsNormal[i + 1] * vsNormal[i + 1] + vsNormal[i + 2] * vsNormal[i + 2]);

					vsNormal[i] /= Eucl;
					vsNormal[i + 1] /= Eucl;
					vsNormal[i + 2] /= Eucl;
				}
			}
			input.close();
			//	cout << vsNormal[0] << vsNormal[1] << vsNormal[2] << endl;

		}
	}
	/*
	void calculateVNormal() {
		vec3* fNorms;
		fNorms = new vec3[iSize];
		for (int i = 0; i < iSize; i++) {
			vec3 u(vCoords[iArray[3 * i + 1] * 3] - vCoords[iArray[3 * i] * 3],
				vCoords[iArray[3 * i + 1] * 3 + 1] - vCoords[iArray[3 * i] * 3 + 1],
				vCoords[iArray[3 * i + 1] * 3 + 2] - vCoords[iArray[3 * i] * 3 + 2]);
			vec3 v(vCoords[iArray[3 * i + 2] * 3] - vCoords[iArray[3 * i + 1] * 3],
				vCoords[iArray[3 * i + 2] * 3 + 1] - vCoords[iArray[3 * i + 1] * 3 + 1],
				vCoords[iArray[3 * i + 2] * 3 + 2] - vCoords[iArray[3 * i + 1] * 3 + 2]);
			fNorms[i] = normalize(cross(u, v));
		}

		for (int i = 0; i < iSize; i++) {
			vfNormal[iArray[3 * i] * 3] = vfNormal[iArray[3 * i + 1] * 3] = vfNormal[iArray[3 * i + 2] * 3] = fNorms[i].x;
			vfNormal[iArray[3 * i] * 3 + 1] = vfNormal[iArray[3 * i + 1] * 3 + 1] = vfNormal[iArray[3 * i + 2] * 3 + 1] = fNorms[i].y;
			vfNormal[iArray[3 * i] * 3 + 2] = vfNormal[iArray[3 * i + 1] * 3 + 2] = vfNormal[iArray[3 * i + 2] * 3 + 2] = fNorms[i].z;
		}

		for (int i = 0; i < 3 * iSize; i++) {
			vsNormal[3 * iArray[i]] += fNorms[i / 3].x;
			vsNormal[3 * iArray[i] + 1] += fNorms[i / 3].y;
			vsNormal[3 * iArray[i] + 2] += fNorms[i / 3].z;
		}
		for (int i = 0; i < vSize; i++) {
			float sum = std::sqrt(vsNormal[3 * i] * vsNormal[3 * i] + vsNormal[3 * i + 1] * vsNormal[3 * i + 1] + vsNormal[3 * i + 2] * vsNormal[3 * i + 2]);
			vsNormal[3 * i] /= sum;
			vsNormal[3 * i + 1] /= sum;
			vsNormal[3 * i + 2] /= sum;
		}

		delete fNorms;
	}*/

};

