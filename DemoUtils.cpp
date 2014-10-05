#include "DemoUtils.h"
#include "ManCam.h"
#include "XmlWriter.h"
#include <Utils/Randomizer.h>
#include <Utils/StringUtils.h>
#include <stdint.h>
#include <XML/XMLLoader.h>
#include <XML/XMLNode.h>
#include <fstream>

sm::Vec3 DemoUtils::GetRandomVector()
{
	static Randomizer random;

	return sm::Vec3(
		random.GetFloat(-1, 1),
		random.GetFloat(-1, 1),
		random.GetFloat(-1, 1)).GetNormalized();
}

float DemoUtils::GetPathLength(std::vector<sm::Vec3>& path)
{
	float length = 0.0f;

	for (uint32_t i = 0; i < path.size() - 1; i++)
		length += (path[i] - path[i + 1]).GetLength();

	return length;
}

sm::Matrix DemoUtils::GetTriangleTransform(sm::Vec3* triangle)
{
	sm::Vec3 center = (triangle[0] + triangle[1] + triangle[2]) / 3.0f;

	sm::Vec3 normal = -((triangle[1] - triangle[0]) * (triangle[2] - triangle[0])).GetNormalized();
	sm::Vec3 triangleBase = triangle[1] - triangle[0];

	sm::Vec3 up = (normal * triangleBase).GetNormalized();

	sm::Matrix rotate;
	if (normal != sm::Vec3(0, 0, 0) &&
		up != sm::Vec3(0, 0, 0))
		rotate = sm::Matrix::CreateLookAt(normal, up);
	else
		rotate = sm::Matrix::Identity;

	return
		sm::Matrix::TranslateMatrix(center) *
		rotate;
}

std::string DemoUtils::ToText(const sm::Vec3& value, const std::string& separator)
{
	char text[128];
	sprintf(text, "%f%s%f%s%f", value.x, separator.c_str(), value.y, separator.c_str(), value.z, separator.c_str());
	return text;
}
std::string DemoUtils::ToText(const sm::Vec4& value, const std::string& separator)
{
	char text[128];
	sprintf(text, "%f%s%f%s%f%s%f", value.x, separator.c_str(), value.y, separator.c_str(), value.z, separator.c_str(), value.w, separator.c_str());
	return text;
}

sm::Vec3 DemoUtils::ParseVector3(const std::string& vectorStr, const std::string& separator)
{
	sm::Vec3 vec;
	sscanf(vectorStr.c_str(), "%f;%f;%f", &vec.x, &vec.y, &vec.z);
	return vec;
}

sm::Vec4 DemoUtils::ParseVector4(const std::string& vectorStr, const std::string& separator)
{
	sm::Vec4 vec;
	sscanf(vectorStr.c_str(), "%f;%f;%f;%f", &vec.x, &vec.y, &vec.z, &vec.w);
	return vec;
}

void DemoUtils::SaveCamera(ManCam* camera, int slot)
{
	char camFilename[128];
	sprintf(camFilename, "cam%d", slot);
	std::ofstream camFile(camFilename);

	XmlWriter xml(&camFile, 0);

	xml.OpenElement("Camera");
	xml.OpenElement("Transform");
	xml.WriteAttribute("position", DemoUtils::ToText(camera->position));
	xml.WriteAttribute("yaw", camera->yaw);
	xml.WriteAttribute("pitch", camera->pitch);
	xml.WriteAttribute("roll", camera->roll);
	xml.CloseElement();
	xml.CloseElement();

	//sm::Matrix transform = camera->GetViewMatrix().GetInversed();
	/*
	char transformText[1024];
	sprintf(
	transformText,
	"%.4f;%.4f;%.4f;%.4f;%.4f;%.4f;%.4f;%.4f;%.4f;%.4f;%.4f;%.4f;%.4f;%.4f;%.4f;%.4f",
	transform.a[0],
	transform.a[1],
	transform.a[2],
	transform.a[3],
	transform.a[4],
	transform.a[5],
	transform.a[6],
	transform.a[7],
	transform.a[8],
	transform.a[9],
	transform.a[10],
	transform.a[11],
	transform.a[12],
	transform.a[13],
	transform.a[14],
	transform.a[15]);
	*/

	camFile.close();
}

void DemoUtils::LoadCamera(ManCam* camera, int slot)
{
	char camFilename[128];
	sprintf(camFilename, "cam%d", slot);

	XMLNode* root = XMLLoader::LoadFromFile(camFilename);
	if (root == NULL)
		return;

	XMLNode* transformNode = root->GetChild("Transform");
	if (transformNode == NULL)
		return;

	camera->position = ParseVector3(transformNode->GetAttribAsString("position"));
	camera->yaw = transformNode->GetAttribAsFloat("yaw");
	camera->pitch = transformNode->GetAttribAsFloat("pitch");
	camera->roll = transformNode->GetAttribAsFloat("roll");
}