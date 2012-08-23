#include "ObjLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

void printDebug(vector<vec3> &positions, vector<int> &indices);

vec3 toVec3(istringstream &iss){
	float x,y,z;
	iss >> x;
	iss >> y;
	iss >> z;
	return vec3(x,y,z);
}

vec2 toVec2(istringstream &iss){
	float x,y;
	iss >> x;
	iss >> y;
	return vec2(x,y);
}

struct TriangleIndex{
	int position;
	int normal;
	int uv;

	bool replace(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = str.find(from);
		if(start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}

	TriangleIndex(string p):position(-1),normal(-1),uv(-1) {
		// position/uv/normal
		replace(p, "//","/-1/");
		stringstream ss(p);
		char buffer[50];

		ss.getline(buffer,50, '/');
		position = atoi(buffer);
		if (ss.good()){
			ss.getline(buffer,50, '/');
			uv = atoi(buffer);
		}
		if (ss.good()){
			ss.getline(buffer,50, '/');
			normal = atoi(buffer);
		}
	}

	// needed to use TriangleIndex as key in map
	bool operator <(const TriangleIndex& Rhs) const {
		return (position < Rhs.position) ||
			(position == Rhs.position && normal < Rhs.normal) ||
			(position == Rhs.position && normal == Rhs.normal && uv < Rhs.uv);      
	}
};

struct TriangleString{
	TriangleIndex v0;
	TriangleIndex v1;
	TriangleIndex v2;

	TriangleString(string v0, string v1, string v2):v0(v0),v1(v1),v2(v2){
	}

	TriangleIndex get(int index){
		if (index == 0) {
			return v0;
		} else if (index == 1) {
			return v1;
		} 
		return v2;
	}
};

bool loadObject(const char * filename, 
	vector<vec3> &outPositions, 
	vector<int> &outIndices,
	vector<vec3> &outNormal, 
	vector<vec2> &outUv,
	float scale){
	
	vector<vec3> positions;
	vector<vec3> normals;
	vector<vec2> uvs;


	vector<TriangleString> triangles;
	ifstream ifs ( filename , ifstream::in );
	char buffer[512];
	while (ifs.good()){
		ifs.getline(buffer,512);
		
		string line(buffer);
		istringstream iss(line);
		string token;
		iss >> token;
		if (token.compare("o")==0){
			// does not support multiple objects
		} else if (token.compare("usemtl")==0){
			// does not support multiple materials
		} else if (token.compare("v")==0){
			positions.push_back( toVec3(iss));
		} else if (token.compare("vn")==0){
			normals.push_back( toVec3(iss));
		} else if (token.compare("vt")==0){
			uvs.push_back( toVec2(iss));
		} else if (token.compare("f")==0){
			vector<string> polygon;
			do {
				string index;
				iss >> index;
				if (index.length() > 0) {
					polygon.push_back(index);
				}
			} while (iss);

			// triangulate pologon
			TriangleString triangle(polygon[0], polygon[1], polygon[2]);
			triangles.push_back(triangle);
			for (int i=3;i<polygon.size();i++){
				TriangleString triangle2(polygon[i-1], polygon[i], polygon[0]);
				triangles.push_back(triangle2);
			}
		}
	}
	ifs.close();

	map<TriangleIndex,int> cache;
	for (int i=0;i<triangles.size();i++){
		TriangleString triangleString = triangles[i];
		for (int j=0;j<3;j++){
			TriangleIndex index = triangleString.get(j);
			map<TriangleIndex,int>::iterator cachedIndex = cache.find(index);
			if (cachedIndex != cache.end()) {
				outIndices.push_back(cachedIndex->second);
			} else {
				int vertexIndex = outPositions.size();
				outPositions.push_back(positions[index.position-1] * scale);
				if (index.normal != -1){
					outNormal.push_back(normals[index.normal-1]);
				}
				if (index.uv != -1) {
					outUv.push_back(uvs[index.uv-1]);
				}
				outIndices.push_back(vertexIndex);
				cache[index] = vertexIndex;
			}
		}
	}
	//cout <<"Indices "<< outIndices.size() << endl;
	//cout <<"Positions "<< outPositions.size() << endl;
	//printDebug(outPositions, outIndices);
	return true;
}

void printDebug(vector<vec3> &positions, vector<int> &indices){
	for (int i=0;i<indices.size();i++){
		cout << positions[indices[i]] <<" ";
		if ((i+1)%3==0){
			cout << endl;
		}
	}
}
