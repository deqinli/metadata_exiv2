#include "utils.h"
#include <iostream>
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <ctime>
#include <fstream>


MetadataUtils::MetadataUtils()
{
}

MetadataUtils::~MetadataUtils()
{
}

std::string MetadataUtils::FindExifKey(Exiv2::ExifData& ed, std::string key)
{
	Exiv2::ExifKey tmp = Exiv2::ExifKey(key);
	Exiv2::ExifData::iterator pos = ed.findKey(tmp);
	if (pos == ed.end())
	{
		return "Unknow";
	}
	return pos->value().toString();
}

void MetadataUtils::readMeta0(const std::string image_path)
{

    Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(image_path);
    assert(image.get() != 0);
    image->readMetadata();
    Exiv2::ExifData& exifData = image->exifData();
    if (exifData.empty()) {
        std::string error(image_path);
        std::cout<< __FUNCTION__<<"::"<<__LINE__<<":: No Exif data found in the file";
        return;
    }
    Exiv2::ExifData::const_iterator end = exifData.end();
    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
        const char* tn = i->typeName();
        std::cout << std::setw(44) << std::setfill(' ') << std::left
            << i->key() << " "
            << "0x" << std::setw(4) << std::setfill('0') << std::right
            << std::hex << i->tag() << " "
            << std::setw(9) << std::setfill(' ') << std::left
            << (tn ? tn : "Unknown") << " "
            << std::dec << std::setw(3)
            << std::setfill(' ') << std::right
            << i->count() << "  "
            << std::dec << i->value()
            << "\n";
    }

}

void MetadataUtils::readMeta1(const std::string image_path)
{
	//输入的图像路径
	static std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(image_path);//尽量别这么写
	//Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(image_path);

	if (image.get() == 0)
	{
		std::cout << "Read Exif Error." << std::endl;
		return;
	}

	//读取照片的exif信息  
	image->readMetadata();
	Exiv2::ExifData ed = image->exifData();//得到exif信息  


	if (ed.empty())
	{
		std::cout << "Not Find ExifInfo" << std::endl;
		return;
	}

	std::cout << "版权	:" << FindExifKey(ed, "Exif.Image.Copyright") << std::endl;
	std::cout << "创作人	:" << FindExifKey(ed, "Exif.Image.Artist") << std::endl;
	std::cout << "相机品牌	:" << FindExifKey(ed, "Exif.Image.Make") << std::endl;
	std::cout << "相机型号	:" << FindExifKey(ed, "Exif.Image.Model") << std::endl;
	std::cout << "镜头型号	:" << FindExifKey(ed, "Exif.Photo.LensModel") << std::endl;
	std::cout << "镜头序列号	:" << FindExifKey(ed, "Exif.Photo.LensSerialNumber") << std::endl;
	std::cout << "ISO	:" << FindExifKey(ed, "Exif.Photo.ISOSpeedRatings") << std::endl;
	std::cout << "光圈	:" << FindExifKey(ed, "Exif.Photo.FNumber") << std::endl;
	std::cout << "焦距	:" << FindExifKey(ed, "Exif.Photo.FocalLength") << std::endl;
	std::cout << "快门	:" << FindExifKey(ed, "Exif.Photo.ExposureTime") << std::endl;
	std::cout << "拍摄时间	:" << FindExifKey(ed, "Exif.Image.DateTime") << std::endl;
	std::cout << "闪光灯	:" << FindExifKey(ed, "Exif.CanonCs.FlashMode") << std::endl;
	std::cout << "水平分辨率	:" << FindExifKey(ed, "Exif.Image.XResolution") << std::endl;
	std::cout << "垂直分辨率	:" << FindExifKey(ed, "Exif.Image.YResolution") << std::endl;
	std::cout << "照片尺寸	:" << FindExifKey(ed, "Exif.Photo.PixelYDimension") << " x " << FindExifKey(ed, "Exif.Photo.PixelXDimension") << std::endl;
}

void MetadataUtils::readMeta2(const std::string image_path)
{

    std::string file = image_path;
    // Container for exif metadata. This is an example of creating
    // exif metadata from scratch. If you want to add, modify, delete
    // metadata that exists in an image, start with ImageFactory::open
    Exiv2::ExifData exifData;
    // *************************************************************************
    // Add to the Exif data
    // This is the quickest way to add (simple) Exif data. If a metadatum for
    // a given key already exists, its value is overwritten. Otherwise a new
    // tag is added.
    exifData["Exif.Image.Model"] = "ncsmb";                     // AsciiValue
    exifData["Exif.Image.SamplesPerPixel"] = uint16_t(162);      // UShortValue
    exifData["Exif.Image.XResolution"] = int32_t(-2);            // LongValue
    exifData["Exif.Image.YResolution"] = Exiv2::Rational(-2, 3); // RationalValue
    std::cout << "Added a few tags the quick way.\n";
    // Create a ASCII string value (note the use of create)
    //Exiv2::Value::AutoPtr v = Exiv2::Value::create(Exiv2::asciiString);
    auto v = Exiv2::Value::create(Exiv2::asciiString);
    // Set the value to a string
    v->read("1999:12:31 23:59:59");
    // Add the value together with its key to the Exif data container
    Exiv2::ExifKey key("Exif.Photo.DateTimeOriginal");
    exifData.add(key, v.get());
    std::cout << "Added key \"" << key << "\", value \"" << *v << "\"\n";

    //*add my tags [ldq 2025-7-10 15:35:56]
    /*auto myTag = Exiv2::Value::create(Exiv2::asciiString);
    v->read("www.ncsmb.com");
    key = Exiv2::ExifKey("Exif.Photo.Company");
    exifData.add(key, myTag.get());*/
    //exifData["Exif.GPSInfo.GPSLatitudeRef"] = Exiv2::AsciiValue("N");

    std::string str = "compant=www.ncsmb.com\n"
        "voltage=15kv\n"
        "WD=8mm\n"
        "pixsize=13.0um\n";
    exifData["Exif.Photo.MakerNote"] = Exiv2::AsciiValue(str);



    // Now create a more interesting value (without using the create method)
    //Exiv2::URationalValue::AutoPtr rv(new Exiv2::URationalValue);
    Exiv2::URationalValue::UniquePtr rv(new Exiv2::URationalValue);
    // Set two rational components from a string
    rv->read("1/2 1/3");
    // Add more elements through the extended interface of rational value
    rv->value_.push_back(std::make_pair(2, 3));
    rv->value_.push_back(std::make_pair(3, 4));
    // Add the key and value pair to the Exif data
    key = Exiv2::ExifKey("Exif.Image.PrimaryChromaticities");
    exifData.add(key, rv.get());
    std::cout << "Added key \"" << key << "\", value \"" << *rv << "\"\n";
    // *************************************************************************
    // Modify Exif data
    // Since we know that the metadatum exists (or we don't mind creating a new
    // tag if it doesn't), we can simply do this:
    Exiv2::Exifdatum& tag = exifData["Exif.Photo.DateTimeOriginal"];
    std::string date = tag.toString();
    date.replace(0, 4, "2000");
    tag.setValue(date);
    std::cout << "Modified key \"" << tag.key()
        << "\", new value \"" << tag.value() << "\"\n";
    // Alternatively, we can use findKey()
    key = Exiv2::ExifKey("Exif.Image.PrimaryChromaticities");
    Exiv2::ExifData::iterator pos = exifData.findKey(key);
    //if (pos == exifData.end()) throw Exiv2::Error(1, "Key not found");
    //if (pos == exifData.end()) throw Exiv2::E
    // Get a pointer to a copy of the value
    v = pos->getValue();
    // Downcast the Value pointer to its actual type
    Exiv2::URationalValue* prv = dynamic_cast<Exiv2::URationalValue*>(v.release());
    ////if (prv == 0) throw Exiv2::Error(1, "Downcast failed");
    rv = Exiv2::URationalValue::UniquePtr(prv);
    // Modify the value directly through the interface of URationalValue
    rv->value_[2] = std::make_pair(88, 77);
    // Copy the modified value back to the metadatum
    pos->setValue(rv.get());
    std::cout << "Modified key \"" << key
        << "\", new value \"" << pos->value() << "\"\n";
    // *************************************************************************
    // Delete metadata from the Exif data container
    // Delete the metadatum at iterator position pos
    key = Exiv2::ExifKey("Exif.Image.PrimaryChromaticities");
    pos = exifData.findKey(key);
    ////if (pos == exifData.end()) throw Exiv2::Error(1, "Key not found");
    exifData.erase(pos);
    std::cout << "Deleted key \"" << key << "\"\n";
    // *************************************************************************
    // Finally, write the remaining Exif data to the image file
    Exiv2::Image::UniquePtr image = Exiv2::ImageFactory::open(file);
    assert(image.get() != 0);
    image->setExifData(exifData);
    image->writeMetadata();
}

void MetadataUtils::readMeta_xmp(const std::string image_path)
{
    const char* file_path = image_path.c_str();

    try {
        // 初始化 Exiv2 库（0.27.0 版本后必需）
        Exiv2::XmpParser::initialize();
        ::atexit(Exiv2::XmpParser::terminate);

        // 打开图像文件
        auto image = Exiv2::ImageFactory::open(file_path);
        if (!image.get()) {
            //throw Exiv2::Error(Exiv2::kerErrorMessage, "无法打开图像文件");
        }

        // 读取元数据
        image->readMetadata();

        // 获取 XMP 数据
        Exiv2::XmpData& xmpData = image->xmpData();
        if (xmpData.empty()) {
            std::cout << "图像中未找到 XMP 数据\n";
            return;
        }

        // 打印 XMP 信息
        std::cout << "文件: " << file_path << "\n";
        std::cout << "发现 " << xmpData.count() << " 个 XMP 属性\n\n";

        // 设置输出格式
        const int keyWidth = 40;
        const int typeWidth = 12;

        std::cout << std::left
            << std::setw(keyWidth) << "属性名称"
            << std::setw(typeWidth) << "类型"
            << "值\n";
        std::cout << std::setfill('-')
            << std::setw(keyWidth) << ""
            << std::setw(typeWidth) << ""
            << std::setw(30) << "\n"
            << std::setfill(' ');

        // 遍历所有 XMP 属性
        for (Exiv2::XmpData::const_iterator md = xmpData.begin(); md != xmpData.end(); ++md) 
        {

            // 输出属性名称
            std::cout << std::left << std::setw(keyWidth) << md->key();

            // 输出数据类型
            std::cout << std::left << std::setw(typeWidth)
                << Exiv2::TypeInfo::typeName(md->typeId());

            // 输出属性值（特殊处理数组和结构类型）
            if (md->typeId() == Exiv2::xmpBag ||
                md->typeId() == Exiv2::xmpSeq ||
                md->typeId() == Exiv2::xmpAlt) {
                // 处理数组类型
                std::cout << "[";
                for (int i = 0; i < md->count(); i++) {
                    if (i > 0) std::cout << ", ";
                    std::cout << "\"" << md->toString(i) << "\"";
                }
                std::cout << "]";
            }
            else if (md->typeId() == Exiv2::xmpText &&
                md->value().size() > 60) {
                // 处理长文本（截断显示）
                std::string value = md->toString();
                std::cout << "\"" << value.substr(0, 57) << "...\"";
            }
            else {
                // 标准值输出
                std::cout << md->toString();
            }

            std::cout << "\n";
        }

        // 打印原始 XMP 包（可选）
        /*std::cout << "\n原始 XMP 包:\n";
        std::cout << "--------------------------------------------------\n";
        std::string xmpPacket;
        if (Exiv2::XmpParser::encode(xmpPacket, xmpData) == 0) {
            std::cout << xmpPacket << "\n";
        }
        else {
            std::cout << "无法生成原始 XMP 包\n";
        }*/

    }
    catch (Exiv2::Error& e) {
        std::cout << "Exiv2 异常: '" << e.what() << "'\n";
        return;
    }
    catch (std::exception& e) {
        std::cout << "标准异常: '" << e.what() << "'\n";
        return;
    }
}

#if 1
void MetadataUtils::writeMeta_xmp(const std::string image_path)
{
    const char* file_path = image_path.c_str();

    try {
        // 初始化 Exiv2 库
        Exiv2::XmpParser::initialize();
        ::atexit(Exiv2::XmpParser::terminate);

        // 打开图像文件
        auto image = Exiv2::ImageFactory::open(file_path);
        if (!image.get()) {
            //throw Exiv2::Error(Exiv2::kerErrorMessage, "无法打开图像文件");
        }

        // 读取现有元数据
        image->readMetadata();

        // 获取 XMP 数据对象（如果不存在则创建）
        Exiv2::XmpData& xmpData = image->xmpData();
        if (xmpData.empty()) {
            std::cout << "create new xmp data\n";
        }

        // 1. 添加标准 XMP 字段 ======================================
        // 作者信息
        xmpData["Xmp.dc.creator"] = "zhangsan";

        // 版权信息
        xmpData["Xmp.dc.rights"] = "© 2023 reserved";

        // 标题和描述（多语言）
        //Exiv2::LangAltValue description;
        //description.setValue("美丽的山水风景", "x-default");
        //description.setValue("Beautiful landscape", "en-US");
        //xmpData["Xmp.dc.description"] = description;

        // 创建日期
        xmpData["Xmp.xmp.CreateDate"] = "2023-08-15T14:30:45";

        // 评分（1-5）
        xmpData["Xmp.xmp.Rating"] = static_cast<uint32_t>(4);

        // 位置信息
        xmpData["Xmp.photoshop.City"] = "kunming";
        xmpData["Xmp.photoshop.State"] = "yunnan";
        xmpData["Xmp.photoshop.Country"] = "chinese";

        // 2. 添加自定义 XMP 字段 ===================================
        // 注册自定义命名空间
        //if (!Exiv2::XmpProperties::nsRegistry().find("myNs")) 
        //if (!Exiv2::XmpProperties::NsRegistry().find("myNs"))
        {
            Exiv2::XmpProperties::registerNs("http://www.example.com/ns/myns/", "myNs");
        }

        // 添加自定义字段
        xmpData["Xmp.myNs.CustomField1"] = "selfdefine1";
        xmpData["Xmp.myNs.CustomField2"] = 42.5; // 浮点数

        // 3. 添加数组类型数据 ======================================
        // 关键字列表（无序数组）
        Exiv2::Value::UniquePtr keywords = Exiv2::Value::create(Exiv2::xmpBag);
        keywords->read("fengguang");
        keywords->read("mountain");
        keywords->read("yunnan");
        xmpData.add(Exiv2::XmpKey("Xmp.dc.subject"), keywords.get());

        // 处理步骤（有序数组）
        Exiv2::Value::UniquePtr steps = Exiv2::Value::create(Exiv2::xmpSeq);
        steps->read("yuanshizhaopian");
        steps->read("tiaozhengbaoguang");
        steps->read("secaijiaozheng");
        xmpData.add(Exiv2::XmpKey("Xmp.myNs.ProcessingSteps"), steps.get());

        // 4. 添加结构化数据 ========================================
        // 创建联系人信息结构
        Exiv2::XmpData::iterator pos = xmpData.findKey(Exiv2::XmpKey("Xmp.myNs.ContactInfo"));
        if (pos == xmpData.end()) {
            // 创建新结构
            //Exiv2::Value::UniquePtr contactStruct = Exiv2::Value::create(Exiv2::xmpStruct);
            //contactStruct->read("Name=张三; Email=zhangsan@example.com; Phone=1234567890");
            //xmpData.add(Exiv2::XmpKey("Xmp.myNs.ContactInfo"), contactStruct.get());
        }

        // 5. 写入元数据并保存文件 =================================
        // 设置修改日期
        xmpData["Xmp.xmp.ModifyDate"] = "2025.07.11 10:49:30";

        // 更新图像的 XMP 数据
        image->setXmpData(xmpData);

        // 写入文件
        image->writeMetadata();

        std::cout << "xmp information has been writen into: " << file_path << "\n";

        // 6. 验证写入结果 =========================================
        std::cout << "\n validation information:\n";
        Exiv2::XmpData& newXmpData = image->xmpData();
        for (Exiv2::XmpData::const_iterator md = newXmpData.begin();
            md != newXmpData.end(); ++md) {
            std::cout << std::left << std::setw(35) << md->key()
                << " = " << md->toString() << "\n";
        }

        // 打印原始 XMP 包
        std::string xmpPacket;
        if (Exiv2::XmpParser::encode(xmpPacket, newXmpData) == 0) {
            std::cout << "\n original xmp data:\n";
            std::cout << "----------------------------------------\n";
            std::cout << xmpPacket << "\n";
        }

    }
    catch (Exiv2::Error& e) {
        std::cout << "Exiv2 exception: '" << e.what() << "'\n";
        return;
    }
    catch (std::exception& e) {
        std::cout << "standard exception: '" << e.what() << "'\n";
        return;
    }
}
#endif


void MetadataUtils::writeJsonToMeta_xmp(const std::string image_path, std::string jsonFile)
{
    const char* imagePath = image_path.c_str();
    const char* jsonSource = jsonFile.c_str();
    const char* outputPath = image_path.c_str();

    bool isOutputDifferent = (strcmp(imagePath, outputPath) != 0);

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
        //if (jsonData.empty())  
        {
            //std::cout << "警告: 未解析到有效的JSON数据，将存储原始字符串\n";
            //jsonData["RawJson"] = jsonStr;
        }

        // 打开图像文件
        auto image = Exiv2::ImageFactory::open(imagePath);
        if (!image.get()) {
            //throw Exiv2::Error(Exiv2::kerErrorMessage, "无法打开图像文件");
            return;
        }

        // 读取现有元数据
        image->readMetadata();

        // 获取 XMP 数据
        Exiv2::XmpData xmpData = image->xmpData();

        // 写入 JSON 数据到 XMP
        //writeJsonToXmp(xmpData, jsonData);
        writeComplexJsonToXmp(xmpData, jsonStr);

        // 更新 XMP 修改时间
        std::time_t now = std::time(nullptr);
        std::tm* local_time = std::localtime(&now);
        char time_str[20];
        std::strftime(time_str, sizeof(time_str), "%Y:%m:%d %H:%M:%S", local_time);
        xmpData["Xmp.xmp.ModifyDate"] = time_str;

        // 设置回图像
        image->setXmpData(xmpData);

        // 写入到文件
        if (isOutputDifferent) {
            // 创建新文件
            //image->writeMetadata(outputPath);
            std::cout << "已创建新文件: " << outputPath << "\n";
        }
        else {
            // 覆盖原文件
            image->writeMetadata();
            std::cout << "已更新文件: " << imagePath << "\n";
        }

        // 验证写入
        std::cout << "\n写入的 JSON 元数据:\n";
        std::cout << "----------------------------------------\n";
        for (const auto jPara : jsonData) {
            std::cout << jPara.first << ": " << jPara.second << "\n";
        }

        // 显示 XMP 中的 JSON 字段
        auto savedImage = Exiv2::ImageFactory::open(outputPath);
        savedImage->readMetadata();
        Exiv2::XmpData& savedXmp = savedImage->xmpData();

        std::cout << "\nXMP 中的 JSON 原始数据:\n";
        auto it = savedXmp.findKey(Exiv2::XmpKey("Xmp.ncsmb.RawJson"));
        if (it != savedXmp.end()) {
            std::cout << it->toString() << "\n";
        }
        else {
            std::cout << "未找到原始 JSON 数据\n";
        }

        std::cout << "\nXMP 中的结构化 JSON 数据:\n";
        for (Exiv2::XmpData::const_iterator md = savedXmp.begin();
            md != savedXmp.end(); ++md) {
            if (md->key().find("Xmp.ncsmb.") == 0) {
                std::cout << std::setw(30) << std::left << md->key()
                    << " = " << md->toString() << "\n";
            }
        }

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

std::map<std::string, std::string> MetadataUtils::parseSimpleJson(const std::string& jsonStr) 
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


// 注册自定义命名空间
void registerJsonNamespace(std::string names_pace = "json") {
    //const std::string NAMESPACE_URI = "http://ns.example.com/json/1.0/";
    const std::string NAMESPACE_URI = "http://ns.example.com/ncsmb/1.0/";
    const std::string PREFIX = names_pace;

    //if (!Exiv2::XmpProperties::NsRegistry().find(PREFIX)) 
    //{
        Exiv2::XmpProperties::registerNs(NAMESPACE_URI, PREFIX);
    //}
}

// 将 JSON 数据写入 XMP
void MetadataUtils::writeJsonToXmp(Exiv2::XmpData& xmpData, const std::map<std::string, std::string>& jsonData) 
{
    registerJsonNamespace();

    // 添加 JSON 原始字符串
    std::string jsonStr = "{";
    for (const auto jPare : jsonData) {
        jsonStr += "\"" + jPare.first + "\":\"" + jPare.second + "\",";
    }
    if (!jsonData.empty()) {
        jsonStr.pop_back(); // 移除最后一个逗号
    }
    jsonStr += "}";

    xmpData["Xmp.json.RawJson"] = jsonStr;

    // 添加结构化字段
    for (const auto jPare : jsonData) {
        // 创建安全的 XMP 键名（替换无效字符）
        std::string safeKey = jPare.first;
        std::replace_if(safeKey.begin(), safeKey.end(),
            [](char c) { return !std::isalnum(c) && c != '_'; }, '_');

        // 添加字段
        std::string xmpKey = "Xmp.json." + safeKey;
        xmpData[xmpKey] = jPare.second;

        // 添加带原始键名的引用
        //xmpData["Xmp.json." + safeKey + "_key"] = jPare.first;
    }
}

void MetadataUtils::readJsonFile(std::string jsonPath)
{
    using json = nlohmann::json;
    // 读取JSON文件
    std::ifstream f(jsonPath);
    if (!f.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }

    try {
        // 解析JSON
        json data = json::parse(f);

        // 访问数据
        std::string name = data["name"];
        int age = data["age"];
        bool is_student = data["is_student"];

        // 访问数组
        auto& courses = data["courses"];
        std::cout << "课程: ";
        for (auto& course : courses) {
            std::cout << course.get<std::string>() << " ";
        }

        // 访问嵌套对象
        auto& address = data["address"];
        std::string city = address["city"];
        std::string street = address["street"];

        // 输出结果
        std::cout << "\n姓名: " << name << "\n年龄: " << age
            << "\n城市: " << city << "\n街道: " << street << std::endl;

        // 检查键是否存在
        if (data.contains("email")) {
            std::cout << "邮箱: " << data["email"] << std::endl;
        }

        // 修改并保存JSON
        data["age"] = 31;
        std::ofstream o("modified.json");
        o << std::setw(4) << data << std::endl;

    }
    catch (const json::parse_error& e) {
        std::cerr << "解析错误: " << e.what() << std::endl;
        return;
    }
}

void MetadataUtils::readJsonFile2(std::string jsonPath)
{
    using json = nlohmann::json;
    // 读取JSON文件
    std::ifstream f(jsonPath);
    if (!f.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }

    try {
        // 解析JSON
        json data = json::parse(f);

        // 访问数据
        std::string name = data["name"];
        int age = data["age"];
        bool is_student = data["is_student"];

        // 访问数组
        auto& courses = data["courses"];
        std::cout << "课程: ";
        for (auto& course : courses) {
            std::cout << course.get<std::string>() << " ";
        }

        // 访问嵌套对象
        auto& address = data["address"];
        std::string city = address["city"];
        std::string street = address["street"];

        // 输出结果
        std::cout << "\n姓名: " << name << "\n年龄: " << age
            << "\n城市: " << city << "\n街道: " << street << std::endl;

        // 检查键是否存在
        if (data.contains("email")) {
            std::cout << "邮箱: " << data["email"] << std::endl;
        }

        // 修改并保存JSON
        data["age"] = 31;
        std::ofstream o("modified.json");
        o << std::setw(4) << data << std::endl;

    }
    catch (const json::parse_error& e) {
        std::cerr << "解析错误: " << e.what() << std::endl;
        return;
    }
}

void MetadataUtils::readJsonFile3(std::string image_path)
{
    // 初始化 Exiv2 库（0.27.0 版本后必需）
    Exiv2::XmpParser::initialize();
    ::atexit(Exiv2::XmpParser::terminate);

    // 显示 XMP 中的 JSON 字段
    auto image = Exiv2::ImageFactory::open(image_path);
    image->readMetadata();
    Exiv2::XmpData& savedXmp = image->xmpData();

    std::cout << "\nXMP 中的 JSON 原始数据:\n";
    auto it = savedXmp.findKey(Exiv2::XmpKey("Xmp.json.RawJson"));
    if (it != savedXmp.end()) {
        std::cout << it->toString() << "\n";
    }
    else {
        std::cout << "未找到原始 JSON 数据\n";
    }


    nlohmann::json jsonParser = readJsonFromXmp(savedXmp);

#if 0
    // 遍历所有 XMP 属性
    for (Exiv2::XmpData::const_iterator md = xmpData.begin(); md != xmpData.end(); ++md)
    {

        // 输出属性名称
        std::cout << std::left << std::setw(keyWidth) << md->key();

        // 输出数据类型
        std::cout << std::left << std::setw(typeWidth) << Exiv2::TypeInfo::typeName(md->typeId());

        // 输出属性值（特殊处理数组和结构类型）
        if (md->typeId() == Exiv2::xmpBag ||
            md->typeId() == Exiv2::xmpSeq ||
            md->typeId() == Exiv2::xmpAlt) {
            // 处理数组类型
            std::cout << "[";
            for (int i = 0; i < md->count(); i++) {
                if (i > 0) std::cout << ", ";
                std::cout << "\"" << md->toString(i) << "\"";
            }
            std::cout << "]";
        }
        else if (md->typeId() == Exiv2::xmpText &&
            md->value().size() > 60) {
            // 处理长文本（截断显示）
            std::string value = md->toString();
            std::cout << "\"" << value.substr(0, 57) << "...\"";
        }
        else {
            // 标准值输出
            std::cout << md->toString();
        }

        std::cout << "\n";
    }
#endif
}

void MetadataUtils::readJsonFromXmp_image(std::string image_path)
{
    // 打开图像文件
    auto image = Exiv2::ImageFactory::open(image_path);

    // 读取现有元数据
    image->readMetadata();
    Exiv2::XmpData& newXmpData = image->xmpData();

    auto ptr = newXmpData.findKey(Exiv2::XmpKey("Xmp.lideqin.RawJson"));
    if (ptr != newXmpData.end())
    {
        std::cout << ptr->toString() << std::endl;

        std::string content = ptr->toString();

        // 创建输出文件流 (默认覆盖模式)
        std::ofstream outFile("output.info");

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
    else
    {
        std::cout << "No json information exist in XmpData." << std::endl;
        return;
    }
    for (Exiv2::XmpData::const_iterator md = newXmpData.begin(); md != newXmpData.end(); ++md) 
    {
        if (md->key().find("Xmp") == 0) 
        {
            std::cout << std::setw(30) << std::left << md->key() << " = " << md->toString() << "\n";
        }
    }

    
}

void MetadataUtils::writeComplexJsonToXmp(Exiv2::XmpData& xmpData, const std::string& jsonStr)
{

    registerJsonNamespace("ncsmb");
    try {
        nlohmann::json json = nlohmann::json::parse(jsonStr);

        // 存储原始 JSON
        xmpData["Xmp.ncsmb.RawJson"] = jsonStr;

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
                xmpData["Xmp.ncsmb." + safeKey] = j.dump();
            }
            };

        process(json, "");

    }
    catch (const std::exception& e) {
        // 解析失败时存储原始字符串
        xmpData["Xmp.ncsmb.RawJson"] = jsonStr;
        xmpData["Xmp.ncsmb.ParseError"] = e.what();
    }
}


nlohmann::json MetadataUtils::readJsonFromXmp(const Exiv2::XmpData& xmpData) 
{
    nlohmann::json json;

    // 尝试获取原始 JSON
    //auto rawIt = xmpData.findKey(Exiv2::XmpKey("Xmp.ncsmb.RawJson"));
    auto it = xmpData.findKey(Exiv2::XmpKey("Xmp.json.RawJson"));
    std::string jsonStr = it->toString();
    std::cout << jsonStr << std::endl;

#if 0
    // 从结构化数据重建 JSON
    std::map<std::string, std::string> keyMap;
    std::map<std::string, std::string> valueMap;

    for (auto it = xmpData.begin(); it != xmpData.end(); ++it) {
        const std::string& key = it->key();

        if (key.find("Xmp.json.") == 0) {
            std::string subKey = key.substr(9); // 移除 "Xmp.json."

            if (subKey.find("_key") == subKey.size() - 4) {
                // 存储键名映射
                std::string baseKey = subKey.substr(0, subKey.size() - 4);
                keyMap[baseKey] = it->toString();
            }
            else {
                // 存储值
                valueMap[subKey] = it->toString();
            }
        }
    }

    // 重建 JSON 对象
    for (const auto& [safeKey, value] : valueMap) {
        std::string originalKey = safeKey;

        // 查找原始键名
        if (keyMap.find(safeKey) != keyMap.end()) {
            originalKey = keyMap[safeKey];
        }

        json[originalKey] = value;
    }

#endif
    return json;
}