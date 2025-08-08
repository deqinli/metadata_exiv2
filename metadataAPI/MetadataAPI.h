#pragma once
#include <string>
#include <memory>

class __declspec(dllexport) MetadataAPI
{
public:
	MetadataAPI();
	~MetadataAPI();
	
	/**
	* @brief ���ļ��ж�ȡjson�ַ���
	* @param strJsonFile
	* @return ��ȡ����json�ַ���
	*/
	std::string getJsonFromJsonFile(const std::string strJsonFile);

	/**
	 * @brief ��ȡ����Ԫ������Ϣ
	 * @param strImagePath ͼ���ļ���·��
	 */
	void readAllMetadata(const std::string strImagePath);

	/**
	 * @brief ��json�ַ���д�뱾���ļ���
	 * @param strSaveFileName ���浽���ص��ļ���
	 * @param strJson ��Ҫ�����json�ַ���
	 */
	void writeJsonToLocalFile(const std::string strSaveFileName, const std::string strJson);

	/**
	 * @brief ��json�ַ���д��ͼ���XmpԪ���ݵ��Զ����ֶΣ�
	 * @param image_path �Ѿ�������Ӳ���ϵ�ͼ���ļ���·����Ϣ��
	 * @param fieldName �Զ����ֶΣ��ֶ���ɸ�ʽΪ Xmp.+ fieldName +.RawJson, ����: Xmp.ncsmb.RawJson,�˴��Զ����ֶ�Ϊncsmb��
	 * @param strRawJson ��Ҫ��ӵ�Json��Ϣ��
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
	 * @brief ɾ��ָ���ֶ�Ԫ������Ϣ
	 * @param strImagePath ͼ���ļ�·��
	 * @param subFieldName �ֶ�ȫ�����磺exif.Image.software
	 */
	void deleteMetadataByfiledName(const std::string strImagePath, std::string subFieldName);

	/**
	 * @brief �������Ԫ������Ϣ
	 * @param strImagePath
	 */
	void clearAllMetadata(const std::string strImagePath);
private:

	class Impl;
	std::unique_ptr<Impl> m_pImpl;
};

