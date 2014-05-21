#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>


 class BadConversion : public std::runtime_error {
 public:
   BadConversion(const std::string& s)
     : std::runtime_error(s)
     { }
 };
 
 //converts a string to a decimal intiger
 inline unsigned int string_to_int(const std::string& s)
 {
   std::istringstream i(s);
   unsigned int x;
   if (!(i >> x))
     throw BadConversion("string_to_int(\"" + s + "\")");
   return x;
 } 

inline int strlength(std::string hi)
 {
  int i;    
  for(i=0;hi[i]!='\0';i++);
  return i;
 }

 //converts a string to a double 
  inline double string_to_double(const std::string& s)
 {
   std::istringstream i(s);
   double x;
   if (!(i >> x))
     throw BadConversion("string_to_double(\"" + s + "\")");
   return x;
 } 
 
 //converts a decimal integer to a string 
   inline std::string int_to_string(const int liczba)
 {
    std::string liczba_str;
    std::stringstream ssx;
    ssx << liczba;
    ssx>>liczba_str;;
//   if (!(liczba >> ssx))
//     throw BadConversion2("int_to_string(\"" + liczba + "\")");
   return liczba_str;
 } 
 
 
 //converts a float to a string 
    inline std::string float_to_string(const float liczba)
 {
    std::string liczba_str;
    std::stringstream ssx;
    ssx << liczba;
    ssx>>liczba_str;;
//   if (!(liczba >> ssx))
//     throw BadConversion2("int_to_string(\"" + liczba + "\")");
   return liczba_str;
 } 
 
 
 //converts a double to a string 
     inline std::string double_to_string(const double liczba)
 {
    std::string liczba_str;
    std::stringstream ssx;
    ssx << liczba;
    ssx>>liczba_str;;
    
   return liczba_str;
 } 


 //converts a decimal integer to a char
    inline char int_to_char(int liczba)
 {
    char liczba_char;
    std::stringstream ssx;
    ssx << liczba;
    ssx>>liczba_char;
//   if (!(ssx>>liczba_char))
//     throw BadConversion("int_to_string(\"" + liczba + "\")");
   return liczba_char;
 }
 
//absolute value of the difference between two numbers |x-y|
  inline float modul_roznicy(float x, float y)
 {
  float z;
   if(x<y)
    {
     z=y-x;
    }
    if(y<x)
     {
      z=x-y;
     }
     
     if(y==x)
     {
      z=0;
     }
   return z;
 } 
 
 //converts a string to a float  
   inline float string_to_float(const std::string& s)
 {
   std::istringstream i(s);
   float x;
   if (!(i >> x))
     throw BadConversion("string_to_double(\"" + s + "\")");
   return x;
 } 

    inline std::string long_to_string(const long liczba)
 {
    std::string liczba_str;
    std::stringstream ssx;
    ssx << liczba;
    ssx>>liczba_str;;
//   if (!(liczba >> ssx))
//     throw BadConversion2("int_to_string(\"" + liczba + "\")");
   return liczba_str;
 } 

