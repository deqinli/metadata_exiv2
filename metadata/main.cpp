#include <iostream> 
#include <string>
#include "../metadataAPI/MetadataAPI.h"


int main()
{
	//MetadataUtils utils;
	MetadataAPI metaApi;
	std::string imagePath = "../bin/0.png";
	std::string jsonPath = "../images/json_SEM1.json";
	//std::string jsonPath2 = "../images/json_read.json";
	//utils.writeJsonToMeta_xmp(imagePath, jsonPath2);
	//utils.readMeta_xmp(imagePath);
	//utils.readJsonFromXmp_image(imagePath);
	
#if 1
	std::string strJson = metaApi.getJsonFromJsonFile(jsonPath);
	//metaApi.writeJsonToXmpByField(imagePath, "lideqin", strJson);
	//metaApi.writeJsonToBmpMetadata2(imagePath, strJson);
	//metaApi.writeJsonToLocalFile("../bin/0_png.data", strJson);
	//metaApi.clearAllMetadata(imagePath);
	metaApi.clearAllMetadata(imagePath);
	//metaApi.deleteMetadataByfiledName(imagePath, "Exif");
	//metaApi.readAllMetadata(imagePath);
	//metaApi.exifWriter_example(imagePath);
	//metaApi.metadataDeletor(imagePath, "json");

#endif
	system("pause");
	return 0;
}
