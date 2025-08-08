#pragma once
#include <string>
#include <memory>

class __declspec(dllexport) MetadataAPI
{
public:
	MetadataAPI();
	~MetadataAPI();
	
	/**
	* @brief 从文件中读取json字符串
	* @param strJsonFile
	* @return 读取到的json字符串
	*/
	std::string getJsonFromJsonFile(const std::string strJsonFile);

	/**
	 * @brief 读取所有元数据信息
	 * @param strImagePath 图像文件的路径
	 */
	void readAllMetadata(const std::string strImagePath);

	/**
	 * @brief 将json字符串写入本地文件中
	 * @param strSaveFileName 保存到本地的文件名
	 * @param strJson 需要保存的json字符串
	 */
	void writeJsonToLocalFile(const std::string strSaveFileName, const std::string strJson);

	/**
	 * @brief 将json字符串写入图像的Xmp元数据的自定义字段，
	 * @param image_path 已经存在于硬盘上的图像文件的路径信息，
	 * @param fieldName 自定义字段，字段组成格式为 Xmp.+ fieldName +.RawJson, 例如: Xmp.ncsmb.RawJson,此处自定义字段为ncsmb，
	 * @param strRawJson 需要添加的Json信息，
	 */
	void writeJsonToXmpByField(const std::string image_path, const std::string fieldName, const std::string strRawJson);

	/**
	 * @brief
	 * @param iamge_path
	 */
	void exifWriter_example(const std::string image_path);

	/**
	 * @brief 
	 * @param image_path 
	 */
	void metadataDeletor(const std::string image_path, std::string nsPrefix);

	/**
	 * @brief 删除指定字段元数据信息
	 * @param strImagePath 图像文件路径
	 * @param subFieldName 字段全名，如：exif.Image.software
	 */
	void deleteMetadataByfiledName(const std::string strImagePath, std::string subFieldName);

	/**
	 * @brief 清除所有元数据信息
	 * @param strImagePath
	 */
	void clearAllMetadata(const std::string strImagePath);
private:

	class Impl;
	std::unique_ptr<Impl> m_pImpl;
};

