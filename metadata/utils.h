#pragma once
#include <exiv2/exiv2.hpp> 
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <nlohmann/json.hpp>

class MetadataUtils
{
public:
	MetadataUtils();
	~MetadataUtils();

	
	
	void readMeta_xmp(const std::string image_path);
	
	void writeJsonToMeta_xmp(const std::string image_path, std::string jsonFile);

	
	void writeJsonToXmp(Exiv2::XmpData& xmpData, const std::map<std::string, std::string>& jsonData);

	void readJsonFile(std::string jsonPath);
	void readJsonFile2(std::string jsonPath);
	void readJsonFile3(std::string jsonPath);
	void readJsonFromXmp_image(std::string image_path);

	void writeComplexJsonToXmp(Exiv2::XmpData& xmpData, const std::string& jsonStr);
	nlohmann::json readJsonFromXmp(const Exiv2::XmpData& xmpData);

private:
	void readMeta0(const std::string image_path);
	void readMeta1(const std::string image_path);
	void readMeta2(const std::string image_path);
	void writeMeta_xmp(const std::string image_path);
	std::string FindExifKey(Exiv2::ExifData& ed, std::string key);
	std::map<std::string, std::string> parseSimpleJson(const std::string& jsonStr);
};

