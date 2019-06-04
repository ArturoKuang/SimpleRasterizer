#include "pch.h"
#include "Mesh.h"
#include <iostream>
#include <fstream>

Mesh::Mesh(std::string pathName)
{
	std::cout << "Loading file named: " << pathName.c_str() << std::endl;

	std::ifstream obj(pathName);

	if (!obj.is_open())
	{
		std::cout << "Unsuccesful at opening the file" << std::endl;
		return;
	}

	std::vector<glm::vec4> positions;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec2> uvs;
	int16_t vertCounter = 0;
	char chars[100];

	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);

		if (chars[0] == 'v' && chars[1] == 'n')
		{
			glm::vec4 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z
			);
			norm.w = 0.0f;

			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			glm::vec2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y
			);

			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			glm::vec4 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z
			);
			pos.w = 1;

			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			int16_t i[12];
			int facesRead = sscanf_s(
				chars,
				"f %hd/%hd/%hd %hd/%hd/%hd %hd/%hd/%hd %hd/%hd/%hd",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8],
				&i[9], &i[10], &i[11]);

			//Create the verts by looking up corresponding
			// data from vectors 
			Vertex v0;
			v0.position = positions[i[0] - 1];
			v0.uv = uvs[i[1] = 1];
			v0.normal = normals[i[2] - 1];

			Vertex v1;
			v1.position = positions[i[3] - 1];
			v1.uv = uvs[i[4] = 1];
			v1.normal = normals[i[5] - 1];

			Vertex v2;
			v2.position = positions[i[6] - 1];
			v2.uv = uvs[i[7] = 1];
			v2.normal = normals[i[8] - 1];

			FaceIndex f0 = { i[0] - 1, i[1] - 1, i[2] - 1 };
			FaceIndex f1 = { i[3] - 1, i[4] - 1, i[5] - 1 };
			FaceIndex f2 = { i[6] - 1, i[7] - 1, i[8] - 1 };


			indices.push_back({ f0, f1, f2 });
			faces.push_back({ v0, v1, v2 });
		}
	}
	obj.close();
}