#include "app.h"


// obfuscate a byte by swap the high 4bit with low 4bit
char obfuscate(char c)
{
    char out = 0;
    out |= ((c & 0xF0) >> 4);
    out |= ((c & 0x0F) << 4);
    return out;
}

// obfuscate a string by obfuscate its every byte
std::string obfuscate(const std::string &in)
{
    std::string out;
    for (char ch : in)
    {
        out.push_back(obfuscate(ch));
    }
    return out;
}

// obfuscate a string twice will deobfuscate
std::string deobfuscate(const std::string &in)
{
    return obfuscate(in);
}

std::string str_reverse(const std::string &in)
{
    std::string out;
    std::reverse_copy(in.begin(), in.end(), std::back_inserter(out));
    return out;
}

std::string base64_encode(const void *data, size_t length)
{
    static const char * code_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string str;
    size_t number_of_blocks = length / 3;
    size_t remainder = length % 3;
    size_t i = 0;
    while (i < number_of_blocks)
    {
        unsigned int A = ((const unsigned char *)(data) + i * 3)[0];
        unsigned int B = ((const unsigned char *)(data) + i * 3)[1];
        unsigned int C = ((const unsigned char *)(data) + i * 3)[2];
        str += code_table[(A & 0xFC) >> 2];
        str += code_table[((A & 0x03) << 4) | ((B & 0xF0) >> 4)];
        str += code_table[((B & 0x0F) << 2) | ((C & 0xC0) >> 6)];
        str += code_table[C & 0x3F];
        ++i;
    }
    switch (remainder)
    {
    case 1:
    {
        unsigned int A = ((const unsigned char *)(data) + i * 3)[0];
        str += code_table[(A & 0xFC) >> 2];
        str += code_table[(A & 0x03) << 4];
        str += "==";
        break;
    }
    case 2:
    {
        unsigned int A = ((const unsigned char *)(data) + i * 3)[0];
        unsigned int B = ((const unsigned char *)(data) + i * 3)[1];
        str += code_table[(A & 0xFC) >> 2];
        str += code_table[((A & 0x03) << 4) | ((B & 0xF0) >> 4)];
        str += code_table[(B & 0x0F) << 2];
        str += "=";
        break;
    }
    default:
        break;
    }
    return str;
}

std::string base64_encode(const std::string &data)
{
    return base64_encode(data.c_str(), data.length());
}

std::string base64_decode(const char *str, size_t length)
{
    // use inverted index to look up original 6bit value from char
    // original 6bit value in [0, 63]
    // '~' is 126 indicates a invalid input
    // '@' is 64 which indicates that program has read a '='
    static const unsigned char inverted_table[] =
    {
        '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', \
        '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', \
        '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', 62, '~', '~', '~', 63, 52, \
        53, 54, 55, 56, 57, 58, 59, 60, 61, '~', '~', '~', '@', '~', '~', '~', 0, 1, 2, 3, \
        4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, \
        '~', '~', '~', '~', '~', '~', 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, \
        39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, '~', '~', '~', '~', '~', '~', \
        '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', \
        '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', \
        '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', \
        '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', \
        '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', \
        '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', \
        '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', \
        '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~', '~'
    };

    if (length % 4 != 0)
        throw std::runtime_error("base64 decodable text length should always be a multiple of 4");

    std::string data; //string is not so suitable here???
    for (size_t i = 0; i < length; i += 4)
    {
        unsigned char W = inverted_table[(unsigned char)str[i]];
        unsigned char X = inverted_table[(unsigned char)str[i + 1]];
        unsigned char Y = inverted_table[(unsigned char)str[i + 2]];
        unsigned char Z = inverted_table[(unsigned char)str[i + 3]];


        data += (W << 2) | (X >> 4); // calculate [W|X]
        if (Y != '@')
        {
            data += ((X & 0x0F) << 4) | (Y >> 2); // calculate [X|Y] if Y is valid
            if (Z != '@')
            {
                data += ((Y & 0x03) << 6) | Z; // calculate [Y|Z] if Y and Z are both valid
            }
        }
    }
    return data;
}

std::string base64_decode(const std::string &str)
{
    return base64_decode(str.c_str(), str.length());
}

void App::init(void)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if (env.value("RELAYD_CONF").isEmpty()) // no conf
    {
        isConfigured = false;
    }
    else if (env.value("RELAYD_CONF") == "Manual") // manual conf
    {
        listenPort = env.value("RELAYD_LISTEN_PORT").toInt();
        forwardAddr = env.value("RELAYD_FORWARD_ADDR");
        forwardPort = env.value("RELAYD_FORWARD_PORT").toInt();
        xferMode = env.value("RELAYD_XFER_MODE").toInt();
        isConfigured = true;
        dump_config();

        QString conf = QString("/%1/%2/%3/%4/")
                .arg(listenPort)
                .arg(forwardAddr).arg(forwardPort)
                .arg(xferMode);
        App::log(QString("You can also use conf: ") + App::encrypt(conf));
    }
    else // lazy conf
    {
        QString conf = env.value("RELAYD_CONF");
        // recover raw base64ed str
        conf = decrypt(conf);
        // no need json, just / separate
        // /0.0.0.0/1500/114.214.167.170/1501/11/
        App::log("Loading lazy conf ...");
        App::log(conf);
        QStringList settings = conf.split("/", QString::SkipEmptyParts);
        listenPort = settings[0].toInt();
        forwardAddr = settings[1];
        forwardPort = settings[2].toInt();
        xferMode = settings[3].toInt();
        isConfigured = true;
        dump_config();
    }
}

// encrypt(obfuscate) a string by doing the following
// 1. obfuscate
// 2. base64
// 3. divide base64ed result to body+suffix, suffix means "==" or "=" or empty
// 4. the encrypted result is reverse(body)+suffix

QString App::encrypt(QString plain_text)
{
    std::string obfuscated_text = obfuscate(plain_text.toStdString());
    std::string base64ed_text = base64_encode(obfuscated_text);
    std::string cipher_text;
    size_t len = base64ed_text.length();
    if (base64ed_text[len - 1] == '=')
    {
        if (base64ed_text[len - 2] == '=')
        {
            base64ed_text.pop_back();
            base64ed_text.pop_back();
            cipher_text = str_reverse(base64ed_text);
            cipher_text.append("==");
        }
        else
        {
            base64ed_text.pop_back();
            cipher_text = str_reverse(base64ed_text);
            cipher_text.append("=");
        }
    }
    else
    {
        cipher_text = str_reverse(base64ed_text);
    }
    QString result = QString::fromStdString(cipher_text);
    return result;
}


// decrypt by doing the following
// 1. divide base64ed result to body+suffix, suffix means "==" or "=" or empty
// 2. plain base64 text is reverse(body)+suffix
// 3. deobfuscate

QString App::decrypt(QString cipher_text)
{
    std::string base64ed_text;
    if (cipher_text.endsWith("=="))
    {
        cipher_text.chop(2);
        base64ed_text = str_reverse(cipher_text.toStdString());
        base64ed_text.append("==");
    }
    else if (cipher_text.endsWith("="))
    {
        cipher_text.chop(1);
        base64ed_text = str_reverse(cipher_text.toStdString());
        base64ed_text.append("=");
    }
    else
    {
        base64ed_text = str_reverse(cipher_text.toStdString());
    }
    std::string obfuscated_text = base64_decode(base64ed_text);
    std::string plain_text = deobfuscate(obfuscated_text);
    QString result = QString::fromStdString(plain_text);
    return result;
}

void App::log_with_time(QString str)
{
    ts << QDateTime::currentDateTime().toString("hh:mm:ss") << "\t" << str << "\n";
    ts.flush();
}

void App::log(QString str)
{
    ts << str << "\n";
    ts.flush();
}

void App::hr()
{
    ts << "=========================" << "\n";
    ts.flush();
}

void App::dump_config()
{
    hr();
    log("Use the Following Configurations:");
    log(QString("Listen Port:\t%1").arg(listenPort));
    log(QString("Forward Addr:\t%1").arg(forwardAddr));
    log(QString("Forward Port:\t%1").arg(forwardPort));
    log(QString("Xfer Mode:\t%1").arg(xferMode));
    hr();
}

bool App::isConfigured;
int App::listenPort;
QString App::forwardAddr;
int App::forwardPort;
int App::xferMode;
uint64_t App::key;
QTextStream App::ts(stdout);
