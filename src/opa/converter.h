
#ifndef OUROBOROS_SCRIPT_CONVERTER_H_
#define OUROBOROS_SCRIPT_CONVERTER_H_


namespace opa {

template <typename T>
class Converter {
  public:
    virtual ~Converter() {}
    
    bool        ScriptToBool    (T t) = 0;
    short       ScriptToShort   (T t) = 0;
    int         ScriptToInt     (T t) = 0;
    float       ScriptToFloat   (T t) = 0;
    double      ScriptToDouble  (T t) = 0;
    char        ScriptToChar    (T t) = 0;
    const char* ScriptToCStr    (T t) = 0;
    
    T BoolToScript      (bool value) = 0;
    T ShortToScript     (short value) = 0;
    T IntToScript       (int value) = 0;
    T FloatToScript     (float value) = 0;
    T DoubleToScript    (double value) = 0;
    T CharToScript      (char value) = 0;
    T CStrToScript      (const char* value) = 0;
 
  protected:
    Converter() {}
};

} /* namespace opa */

#endif /* OUROBOROS_SCRIPT_CONVERTER_H_ */
