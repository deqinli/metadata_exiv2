#include "MetadataAPI.h"
#include <exiv2/exiv2.hpp> 
#include <nlohmann/json.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <exiv2/basicio.hpp>

class MetadataAPI::Impl
{
public:
    /**
     * @brief 读取所有元数据信息
     * @param strImagePath 图像文件的路径
     */
    void readAllMetadata(const std::string strImagePath);

    /**
     * @brief 将json字符串写入图像的Xmp元数据的自定义字段，
     * @param image_path 已经存在于硬盘上的图像文件的路径信息，
     * @param fieldName 自定义字段，字段组成格式为 Xmp.+ fieldName +.RawJson, 例如: Xmp.ncsmb.RawJson,此处自定义字段为ncsmb，
     * @param strRawJson 需要添加的Json信息，
     */
    void writeJsonToXmpByField(const std::string image_path, const std::string fieldName, const std::string strRawJson);

    /**
    * @brief 从文件中读取json字符串
    * @param strJsonFile
    * @return 读取到的json字符串
    */
    std::string getJsonFromJsonFile(const std::string strJsonFile);


    /**
     * @brief 将json字符串写入本地文件中
     * @param strSaveFileName 保存到本地的文件名
     * @param strJson 需要保存的json字符串
     */
    void writeJsonToLocalFile(const std::string strSaveFileName, const std::string strJson);

    /**
     * @brief : write json string to local bmp format image metadata.
     * @param image_path : local image file path.
     * @param strJson : json string needed to be written to image.
     */
    void writeJsonToBmpMetadata(const std::string image_path, const std::string strJson);

    /**
     * @brief : write json string to local bmp format image metadata.
     * @param image_path : local image file path.
     * @param strJson : json string needed to be written to image.
     */
    void writeJsonToBmpMetadata2(const std::string image_path, const std::string strJson);

    /**
     * @brief 
     * @param iamge_path 
     */
    void exifWriter(const std::string iamge_path);

    /**
     * @brief 
     * @param image_path 
     */
    void deleteXmpMetadataByField(const std::string image_path, std::string nsPrefix);

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

    /**
     * @brief 解析简单的Json字符串
     * @param jsonStr 输入，Json字符串
     * @return
     */
    std::map<std::string, std::string> parseSimpleJson(const std::string& jsonStr);

    /**
     * @brief 将复杂的Json字符串写入Xmp元数据信息
     * @param xmpData Xmp元数据信息操作类
     * @param fieldName 字段名称
     * @param jsonStr 需要写入的Json字符串
     */
    void writeComplexJsonToXmp(Exiv2::XmpData& xmpData, std::string fieldName, const std::string& jsonStr);

    /**
     * @brief 注册自定义命名空间
     * @param names_pace 需要注册的字段名，如 "Xmp.ncsmb.RawJson",需要注册ncsmb
     */
    void registerJsonNamespace(std::string names_pace = "ncsmb");

    /**
     * @brief 读取exif数据信息
     * @param strImagePath 图像文件路径
     */
    void readExif(const std::string strImagePath);

    /**
     * @brief 读取xmp数据信息
     * @param strImagePath 图像文件路径
     */
    void readXmp(const std::string strImagePath);

};

MetadataAPI::MetadataAPI() : m_pImpl(std::make_unique<Impl>()){}

MetadataAPI::~MetadataAPI() = default;

void MetadataAPI::Impl::clearAllMetadata(const std::string strImagePath)
{
    std::string filePath = strImagePath;

    // 检查文件是否存在
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "错误: 文件不存在 - " << filePath << "\n";
        return;
    }

    try {
        // 创建备份文件（安全操作）
        std::string backupPath = filePath + ".bak";
        std::filesystem::copy_file(filePath, backupPath, std::filesystem::copy_options::overwrite_existing);
        std::cout << "已创建备份文件: " << backupPath << "\n";

        // 打开图像文件
        Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(filePath);
        if (!image.get()) 
        {
            throw Exiv2::Error(Exiv2::ErrorCode::kerErrorMessage, "无法打开图像文件");
        }

        // 读取元数据（必要步骤）
        image->readMetadata();

        // 检查是否有元数据
        bool hasExif = !image->exifData().empty();
        bool hasIptc = !image->iptcData().empty();
        bool hasXmp = !image->xmpData().empty();
        bool hasComment = !image->comment().empty();

        if (!(hasExif || hasIptc || hasXmp || hasComment)) 
        {
            std::cout << "提示: 文件不包含任何元数据，无需处理\n";
            return;
        }

        std::cout << "原始元数据统计:\n";
        std::cout << "  EXIF: " << image->exifData().count() << " 项\n";
        std::cout << "  IPTC: " << image->iptcData().count() << " 项\n";
        std::cout << "  XMP:  " << image->xmpData().count() << " 项\n";
        std::cout << "  注释: " << (hasComment ? "有" : "无") << "\n";

        // 清除所有类型的元数据
        /*image->clearExifData();
        image->clearIptcData();
        image->clearXmpData();
        image->clearComment();*/
#if 1
        Exiv2::ExifData& exifData = image->exifData();
        Exiv2::ExifData::iterator iter_exif = exifData.begin();
        for (; iter_exif != exifData.end();)
        {
            iter_exif = exifData.erase(iter_exif);
        }
        // 保存修改
        image->writeMetadata();


        image->readMetadata();
        Exiv2::XmpData& xmpData = image->xmpData();
        Exiv2::XmpData::iterator iter_xmp = xmpData.begin();
        for (; iter_xmp != xmpData.end();)
        {
            iter_xmp = xmpData.erase(iter_xmp);
        }
        image->writeMetadata();


        image->readMetadata();
        Exiv2::IptcData& iptcData = image->iptcData();
        Exiv2::IptcData::iterator iter_iptc = iptcData.begin();
        for (; iter_iptc != iptcData.end();)
        {
            iter_iptc = iptcData.erase(iter_iptc);
        }


        // 保存修改
        image->writeMetadata();

        // 验证删除结果
        image->readMetadata();
        bool metadataRemoved = image->exifData().empty() &&
            image->iptcData().empty() &&
            image->xmpData().empty() &&
            image->comment().empty();

        if (metadataRemoved) {
            // 获取原始文件大小和修改后文件大小
            auto originalSize = std::filesystem::file_size(backupPath);
            auto newSize = std::filesystem::file_size(filePath);

            std::cout << "\n成功删除所有元数据!\n";
            std::cout << "文件大小变化: " << originalSize << " 字节 -> "
                << newSize << " 字节 (减少: "
                << (originalSize - newSize) << " 字节)\n";
        }
        else {
            std::cerr << "警告: 未能完全删除所有元数据\n";
            return;
        }
#endif

    }
    catch (const Exiv2::Error& e) {
        std::cerr << "Exiv2 错误: " << e.what() << "\n";
        return;
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "文件系统错误: " << e.what() << "\n";
        return;
    }
    catch (const std::exception& e) {
        std::cerr << "标准错误: " << e.what() << "\n";
        return;
    }
}

void MetadataAPI::Impl::deleteMetadataByfiledName(const std::string strImagePath, std::string subFieldName)
{
    // 初始化 Exiv2（处理 XMP 需要）
    Exiv2::XmpParser::initialize();
    auto cleanup = [] { Exiv2::XmpParser::terminate(); };
    std::atexit(cleanup);

    std::string filePath = strImagePath;

    // 检查文件是否存在
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "错误: 文件不存在 - " << filePath << "\n";
        return;
    }

    // 打开图像
    auto image = Exiv2::ImageFactory::open(filePath);
    assert(image.get() != 0);
    image->readMetadata();  // 读取元数据

    // 根据键前缀判断元数据类型
    if (-1 != subFieldName.find("Exif"))
    {
        Exiv2::ExifData& exifData = image->exifData();
        Exiv2::ExifData::iterator iter = exifData.begin();
        for (; iter != exifData.end();)
        {
            if (-1 != iter->key().find(subFieldName))
            {
                iter = exifData.erase(iter);
            }
            else
            {
                iter++;
            }
        }
    }
    else if (-1 != subFieldName.find("Xmp"))
    {
        Exiv2::XmpData& xmpData = image->xmpData();
        Exiv2::XmpData::iterator iter = xmpData.begin();
        for (; iter != xmpData.end();)
        {
            if (-1 != iter->key().find(subFieldName))
            {
                iter = xmpData.erase(iter);
            }
            else
            {
                iter++;
            }
        }
    }
    else if (-1 != subFieldName.find("Iptc"))
    {
        Exiv2::IptcData& iptcData = image->iptcData();
        Exiv2::IptcData::iterator pos = iptcData.findKey(Exiv2::IptcKey(subFieldName));
        if (pos != iptcData.end()) {
            iptcData.erase(pos);
            std::cout << "已删除 IPTC 字段: " << subFieldName << std::endl;
        }
        else {
            std::cerr << "警告: 未找到 IPTC 键 - " << subFieldName << std::endl;
        }
    }
    else {
        std::cerr << "错误: 无效键前缀 (需以 Exif，或 Iptc，或 Xmp 开头)" << std::endl;
        return;
    }
}

void MetadataAPI::Impl::readAllMetadata(const std::string strImagePath)
{
    readExif(strImagePath);
    readXmp(strImagePath);
}

void MetadataAPI::Impl::readExif(const std::string strImagePath)
{
    // 打开图像文件
    auto image = Exiv2::ImageFactory::open(strImagePath);

    // 读取现有元数据
    image->readMetadata(); 
    Exiv2::ExifData& exifData = image->exifData();

    for (Exiv2::ExifData::const_iterator iter = exifData.begin(); iter != exifData.end(); iter++)
    {
        if (iter->key().find("Exif") == 0)
        {
            std::cout << std::setw(30) << std::left << iter->key() << " = " << iter->toString() << "\n";
        }
    }

}

void MetadataAPI::Impl::readXmp(const std::string strImagePath)
{
    // 打开图像文件
    auto image = Exiv2::ImageFactory::open(strImagePath);

    // 读取现有元数据
    image->readMetadata();
    Exiv2::XmpData& xmpData = image->xmpData();

    for (Exiv2::XmpData::const_iterator iter = xmpData.begin(); iter != xmpData.end(); iter++)
    {
        if (iter->key().find("Xmp") == 0)
        {
            std::cout << std::setw(30) << std::left << iter->key() << " = " << iter->toString() << "\n";
        }
    }
}

void MetadataAPI::Impl::writeJsonToXmpByField(const std::string image_path, const std::string fieldName, const std::string strRawJson)
{
    const char* imagePath = image_path.c_str();
    const char* jsonSource = strRawJson.c_str();

    try {
        // 初始化 Exiv2 库
        Exiv2::XmpParser::initialize();
        ::atexit(Exiv2::XmpParser::terminate);

        // 准备 JSON 数据
        std::string jsonStr;

        // 检查是否是文件路径
        std::ifstream file(jsonSource);
        if (file) {
            jsonStr = std::string((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
        }
        else {
            jsonStr = jsonSource;
        }

        // 解析 JSON（简化版解析器）
        auto jsonData = parseSimpleJson(jsonStr);

        bool isData = jsonData.empty();


        // 打开图像文件
        auto image = Exiv2::ImageFactory::open(imagePath);
        if (!image.get()) {
            throw Exiv2::Error(Exiv2::ErrorCode::kerErrorMessage, "无法打开图像文件");
            return;
        }

        // 读取现有元数据
        image->readMetadata();

        // 获取 XMP 数据
        Exiv2::XmpData xmpData = image->xmpData();

        // 写入 JSON 数据到 XMP
        writeComplexJsonToXmp(xmpData, fieldName, jsonStr);

        // 更新 XMP 修改时间
        std::time_t now = std::time(nullptr);
        std::tm* local_time = std::localtime(&now);
        char time_str[20];
        std::strftime(time_str, sizeof(time_str), "%Y:%m:%d %H:%M:%S", local_time);
        xmpData["Xmp.xmp.ModifyDate"] = time_str;

        // 设置回图像
        image->setXmpData(xmpData);
        image->writeMetadata();

    }
    catch (Exiv2::Error& e) {
        std::cerr << "Exiv2 错误: " << e.what() << "\n";
        return;
    }
    catch (std::exception& e) {
        std::cerr << "错误: " << e.what() << "\n";
        return;
    }
}

std::string MetadataAPI::Impl::getJsonFromJsonFile(const std::string strJsonFile)
{
    // 准备 JSON 数据
    std::string jsonStr;

    // 检查是否是文件路径
    std::ifstream file(strJsonFile);
    if (file) {
        jsonStr = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); 
    }
    else {
        jsonStr = strJsonFile; 
    }
    return jsonStr;
}

void MetadataAPI::Impl::writeJsonToLocalFile(const std::string strSaveFileName, const std::string strJson)
{
    std::string content = strJson;

    // 创建输出文件流 (默认覆盖模式)
    std::ofstream outFile(strSaveFileName,std::ios::binary);

    // 检查文件是否成功打开
    if (!outFile.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }

    // 写入内容
    outFile << content;

    // 显式关闭文件(析构函数会自动调用，但显式关闭更好)
    outFile.close();
}

// 注册自定义命名空间
void MetadataAPI::Impl::registerJsonNamespace(std::string names_pace) {
    //const std::string NAMESPACE_URI = "http://ns.example.com/json/1.0/";
    const std::string NAMESPACE_URI = "http://www.ncsmb.com/"+names_pace+"/1.0/";
    const std::string PREFIX = names_pace;

    //std::string ns = Exiv2::XmpProperties::NsRegistry().find(PREFIX)->first;
    //if (""!=ns)
    {
        Exiv2::XmpProperties::registerNs(NAMESPACE_URI, PREFIX);
    }
}

std::map<std::string, std::string> MetadataAPI::Impl::parseSimpleJson(const std::string& jsonStr)
{
    std::map<std::string, std::string> result;
    std::string key, value;
    bool inKey = false, inValue = false, inString = false;
    char prevChar = 0;

    for (char c : jsonStr) {
        if (c == '"' && prevChar != '\\') {
            inString = !inString;
            continue;
        }

        if (!inString) {
            if (c == '{' || c == '}' || c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                prevChar = c;
                continue;
            }

            if (c == ':') {
                inKey = false;
                inValue = true;
                prevChar = c;
                continue;
            }

            if (c == ',') {
                if (!key.empty() && !value.empty()) {
                    result[key] = value;
                }
                key.clear();
                value.clear();
                inValue = false;
                prevChar = c;
                continue;
            }
        }

        if (inString) {
            if (!inKey && !inValue) {
                inKey = true;
            }

            if (inKey) {
                key += c;
            }
            else if (inValue) {
                value += c;
            }
        }

        prevChar = c;
    }

    // 添加最后一个键值对
    if (!key.empty() && !value.empty()) {
        result[key] = value;
    }

    return result;
}

void MetadataAPI::Impl::writeComplexJsonToXmp(Exiv2::XmpData& xmpData, std::string fieldName, const std::string& jsonStr)
{
    registerJsonNamespace(fieldName);
    // 存储原始 JSON
    std::string filed_123 = "Xmp." + fieldName + ".RawJson";
    std::string fields_12 = "Xmp." + fieldName + ".";
    xmpData[filed_123] = jsonStr;
    try {
        nlohmann::json json = nlohmann::json::parse(jsonStr);

        // 递归处理 JSON
        std::function<void(const nlohmann::json&, const std::string&)> process;

        process = [&](const nlohmann::json& j, const std::string& prefix) {
            if (j.is_object()) {
                for (auto it = j.begin(); it != j.end(); ++it) {
                    std::string newPrefix = prefix + it.key() + ".";
                    process(it.value(), newPrefix);
                }
            }
            else if (j.is_array()) {
                int index = 0;
                for (const auto& item : j) {
                    std::string newPrefix = prefix + std::to_string(index++) + ".";
                    process(item, newPrefix);
                }
            }
            else {
                // 创建安全的 XMP 键名
                std::string safeKey = prefix;
                safeKey.pop_back(); // 移除末尾的点
                std::replace_if(safeKey.begin(), safeKey.end(),
                    [](char c) { return !std::isalnum(c) && c != '_' && c != '.'; }, '_');

                // 存储值
                xmpData[fields_12 + safeKey] = j.dump();
            }
        };

        process(json, "");

    }
    catch (const std::exception& e) {
        // 解析失败时存储原始字符串
        xmpData[filed_123] = jsonStr;
        xmpData[fields_12+"ParseError"] = e.what();
    }
}

void MetadataAPI::Impl::exifWriter(const std::string image_path)
{
    try {

        // 1. 打开图像文件
        auto image = Exiv2::ImageFactory::open(image_path);
        if (!image.get()) {
            throw std::runtime_error("无法打开图像文件: " + image_path);
        }

        // 2. 读取现有元数据
        image->readMetadata();

        // 3. 获取Exif数据对象
        Exiv2::ExifData& exifData = image->exifData();

        std::cout << "\n原始Exif数据:\n";

        // 4. 添加/修改Exif元数据
        // 基本相机信息
        exifData["Exif.Image.Make"] = "Custom Camera";
        exifData["Exif.Image.Model"] = "BMP Image Processor";
        exifData["Exif.Image.Software"] = "exiv2 BMP Writer v1.0";

        // 拍摄日期时间
        exifData["Exif.Image.DateTime"] = "2023:11:15 14:30:45";
        exifData["Exif.Photo.DateTimeOriginal"] = "2023:11:15 14:30:45";
        exifData["Exif.Photo.DateTimeDigitized"] = "2023:11:15 14:31:10";

        // 图像参数
        exifData["Exif.Image.Orientation"] = uint16_t(1); // 正常方向
        exifData["Exif.Image.XResolution"] = Exiv2::Rational(300, 1);
        exifData["Exif.Image.YResolution"] = Exiv2::Rational(300, 1);
        exifData["Exif.Image.ResolutionUnit"] = uint16_t(2); // 英寸

        // 拍摄参数
        exifData["Exif.Photo.ExposureTime"] = Exiv2::Rational(1, 60);
        exifData["Exif.Photo.FNumber"] = Exiv2::Rational(28, 10); // f/2.8
        exifData["Exif.Photo.ISOSpeedRatings"] = uint16_t(400);
        exifData["Exif.Photo.FocalLength"] = Exiv2::Rational(50, 1); // 50mm

        // GPS信息
        exifData["Exif.GPSInfo.GPSVersionID"] = "2 2 0 0";
        exifData["Exif.GPSInfo.GPSLatitudeRef"] = "N";
        exifData["Exif.GPSInfo.GPSLatitude"] = "40/1 42/1 30/1"; // 40°42'30" N
        exifData["Exif.GPSInfo.GPSLongitudeRef"] = "W";
        exifData["Exif.GPSInfo.GPSLongitude"] = "74/1 0/1 0/1"; // 74°0'0" W

        // 自定义Exif字段（使用私有标签）
        // 注意：私有标签可能不被其他软件识别
        exifData["Exif.Image.ProcessingSoftware"] = "Custom BMP Processor";
        exifData["Exif.Photo.UserComment"] = "Custom metadata added by exiv2";

        // 5. 写回元数据
        image->setExifData(exifData);
        image->writeMetadata();

        std::cout << "\n成功写入Exif元数据到: " << image_path << "\n";

        // 6. 重新读取验证
        image->readMetadata();
        std::cout << "\n写入后的Exif数据:\n";
        exifData = image->exifData();
        if (!exifData.empty()) {
            std::cout << "\n exif data :\n";
            for (auto it = exifData.begin(); it != exifData.end(); ++it) {
                std::cout << it->key() << " = " << it->value() << "\n";
            }
        }
    }
    catch (const Exiv2::Error& e) {
        std::cerr << "Exiv2 错误: " << e.what() << "\n";
        return;
    }
    catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << "\n";
        return;
    }
}

void MetadataAPI::Impl::deleteXmpMetadataByField(const std::string image_path, std::string nsPrefix)
{
    try {
        Exiv2::XmpParser::initialize();
        ::atexit(Exiv2::XmpParser::terminate);
        //std::string nsPrefix = "json";
        // 0. 备份原始文件

        // 1. 打开图像文件
        auto image = Exiv2::ImageFactory::open(image_path);
        if (!image.get()) {
            throw std::runtime_error("无法打开图像文件: " + image_path);
        }

        // 2. 读取元数据
        image->readMetadata();
        Exiv2::XmpData& xmpData = image->xmpData();

        // 3. 打印原始元数据
        std::cout << "\n原始元数据 (" << xmpData.count()<< " 个字段):\n";
        for (auto it = xmpData.begin(); it != xmpData.end(); ++it) {
            std::cout << it->key() << " = " << it->value() << "\n";
        }

        // 4. 删除自定义字段
        int removedCount = 0;

        if (nsPrefix.empty()) 
        {
            // 删除所有自定义字段（非标准命名空间）
            for (auto it = xmpData.begin(); it != xmpData.end();) 
            {
                const std::string key = it->key();

                // 检查是否是标准命名空间
                bool isStandard = (key.find("Xmp.dc.") == 0 ||
                    key.find("Xmp.xmp.") == 0 ||
                    key.find("Xmp.tiff.") == 0 ||
                    key.find("Xmp.exif.") == 0 ||
                    key.find("Xmp.iptc.") == 0 ||
                    key.find("Xmp.photoshop.") == 0 ||
                    key.find("Xmp.crs.") == 0);

                if (!isStandard) 
                {
                    std::cout << "删除: " << key << "\n";
                    it = xmpData.erase(it);
                    removedCount++;
                }
                else 
                {
                    ++it;
                }
            }
        }
        else {
            // 删除指定命名空间的字段
            const std::string prefix = "Xmp." + nsPrefix + ".";

            Exiv2::XmpData::iterator it = xmpData.begin();

            for (; it != xmpData.end();)
            {
                if (it->key().find(prefix) == 0) 
                {
                    std::cout << "删除: " << it->key() << "\n";
                    it = xmpData.erase(it);
                    removedCount++;
                }
                else
                {
                    it++;
                }
            }
        }

        // 5. 写回元数据
        if (removedCount > 0) {
            image->setXmpData(xmpData);
            image->writeMetadata();
            std::cout << "\n成功删除 " << removedCount << " 个自定义字段\n";
        }
        else {
            std::cout << "\n未找到匹配的自定义字段\n";
        }

        // 6. 重新读取并验证
        image->readMetadata();
        const Exiv2::XmpData& updatedData = image->xmpData();
        std::cout << "\n剩余元数据 (" << updatedData.count() << " 个字段):\n";

        for (auto it = updatedData.begin(); it != updatedData.end(); ++it) {
            std::cout << it->key() << " = " << it->value() << "\n";
        }

        // 7. 检查命名空间是否完全移除
        if (!nsPrefix.empty() && removedCount > 0) {
            bool nsStillUsed = false;
            const std::string prefix = "Xmp." + nsPrefix + ".";

            for (auto it = updatedData.begin(); it != updatedData.end(); ++it) {
                if (it->key().find(prefix) == 0) {
                    nsStillUsed = true;
                    break;
                }
            }

            if (!nsStillUsed) {
                std::cout << "\n命名空间 '" << nsPrefix << "' 已完全移除\n";
            }
        }

    }
    catch (const Exiv2::Error& e) {
        std::cerr << "Exiv2 错误: " << e.what() << "\n";
        return;
    }
    catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << "\n";
        return;
    }
}

std::string MetadataAPI::getJsonFromJsonFile(const std::string strJsonFile)
{
    return m_pImpl->getJsonFromJsonFile(strJsonFile);
}

void MetadataAPI::readAllMetadata(const std::string strImagePath)
{
    m_pImpl->readAllMetadata(strImagePath);
}

void MetadataAPI::writeJsonToLocalFile(const std::string strSaveFileName, const std::string strJson)
{
    m_pImpl->writeJsonToLocalFile(strSaveFileName, strJson);
}

void MetadataAPI::writeJsonToXmpByField(const std::string image_path, const std::string fieldName, const std::string strRawJson)
{
    m_pImpl->writeJsonToXmpByField(image_path, fieldName, strRawJson);
}

void MetadataAPI::exifWriter_example(const std::string image_path)
{
    m_pImpl->exifWriter(image_path);
}

void MetadataAPI::metadataDeletor(const std::string image_path, std::string nsPrefix)
{
    m_pImpl->deleteXmpMetadataByField(image_path, nsPrefix);
}

void MetadataAPI::deleteMetadataByfiledName(const std::string strImagePath, std::string subFieldName)
{
    m_pImpl->deleteMetadataByfiledName(strImagePath, subFieldName);
}

void MetadataAPI::clearAllMetadata(const std::string strImagePath)
{
    m_pImpl->clearAllMetadata(strImagePath);
}