#pragma once

#include <bdn/Color.h>
#include <bdn/java/Reference.h>
#include <list>
#include <string>

namespace bdn::java
{
    class TypeConversionBase_
    {
      public:
      protected:
        static jobject _createJString(const std::string &s, std::list<Reference> &createdJavaObjects);
        static std::string _getStringFromJava(const Reference &ref);
    };

    /** Template class that converts between native C++ types and their
     *corresponding JNI type.
     *
     *  This is used for converting method arguments and return values.
     *
     *  For many simple types like int, double, etc. the Java type is the
     *same as the C++ type (although there are exceptions like bool).
     *
     *  For classes derived from bdn::java::wrapper::JObject the Java type is
     *jobject.
     *
     *  For some types an automatic convenience conversion is defined. For
     *example, std::string objects will automatically be converted to and
     *from java.lang.std::string objects.
     *
     *  The different conversions are implemented via template
     *specializations.
     *
     **/
    template <typename NATIVE_TYPE> class TypeConversion : public TypeConversionBase_
    {
      public:
        using JavaType = jobject;
        using NativeType = NATIVE_TYPE;

        /** Returns the signature string of the java type.
         *  This is the string that is used in JNI method signature strings.
         *
         *  For simple types it is a single character (e.g. "I" for
         *integers).
         *
         *  For objects the string has the format "LClassName;" where
         *ClassName is the name of the class.
         *
         **/
        static std::string getJavaSignature()
        {
            static std::string sig(NativeType::getStaticClass_().getSignature_());

            return sig;
        }

        /** Converts a natve value to the corresponding java value.
         *
         *  If a new Java object is created then a reference to it is
         *  stored in the createdJavaObjects list. The object references
         * stored in the list must be kept alive as long as the returned
         * java value is needed. Afterwards they can simply be deleted.
         * */
        static JavaType nativeToJava(NativeType arg, std::list<Reference> &createdJavaObjects)
        {
            return arg.getRef_().getJObject();
        }

        /** Converts a java  to the corresponding native value type.
         *  Note that the function will take ownership of the java value.
         *  If it is a jobject then the function will will assume that the
         * jobject reference is local     * and return a corresponding C++
         * object that automatically deletes the local jobject when it (and
         * all the copies of the C++ object that may be created) are
         * destroyed.
         *  */
        static NativeType takeOwnershipOfJavaValueAndConvertToNative(JavaType arg)
        {
            return NativeType(Reference::convertAndDestroyOwnedLocal(arg));
        }
    };

    template <> class TypeConversion<std::string> : public TypeConversionBase_
    {
      public:
        using JavaType = jobject;
        using NativeType = std::string;

        static std::string getJavaSignature()
        {
            static std::string s("Ljava/lang/String;");
            return s;
        }

        static JavaType nativeToJava(const NativeType &arg, std::list<Reference> &createdJavaObjects)
        {
            return _createJString(arg, createdJavaObjects);
        }

        static NativeType takeOwnershipOfJavaValueAndConvertToNative(JavaType arg)
        {
            return _getStringFromJava(Reference::convertAndDestroyOwnedLocal(arg));
        }
    };

    template <> class TypeConversion<bdn::Color> : public TypeConversionBase_
    {
      public:
        using JavaType = jint;
        using NativeType = bdn::Color;

        static std::string getJavaSignature()
        {
            static std::string sig("I");
            return sig;
        }

        static JavaType nativeToJava(NativeType arg, std::list<Reference> &createdJavaObjects)
        {
            return arg.asIntAlphaFirst();
        }

        static NativeType takeOwnershipOfJavaValueAndConvertToNative(JavaType arg)
        {
            return Color::fromIntAlphaFirst(arg);
        }
    };

    template <> class TypeConversion<jobject> : public TypeConversionBase_
    {
      public:
        using JavaType = jobject;
        using NativeType = jobject;

        static std::string getJavaSignature()
        {
            static std::string sig("Ljava/lang/Object;");
            return sig;
        }

        static JavaType nativeToJava(NativeType arg, std::list<Reference> &createdJavaObjects) { return arg; }

        static NativeType takeOwnershipOfJavaValueAndConvertToNative(JavaType arg) { return arg; }
    };

    template <> class TypeConversion<int> : public TypeConversionBase_
    {
      public:
        using JavaType = jint;
        using NativeType = int;

        static std::string getJavaSignature()
        {
            static std::string sig("I");
            return sig;
        }

        static JavaType nativeToJava(NativeType arg, std::list<Reference> &createdJavaObjects) { return arg; }

        static NativeType takeOwnershipOfJavaValueAndConvertToNative(JavaType arg) { return arg; }
    };

    template <> class TypeConversion<short> : public TypeConversionBase_
    {
      public:
        using JavaType = jshort;
        using NativeType = short;

        static std::string getJavaSignature()
        {
            static std::string sig("S");
            return sig;
        }

        static JavaType nativeToJava(NativeType arg, std::list<Reference> &createdJavaObjects) { return arg; }

        static NativeType takeOwnershipOfJavaValueAndConvertToNative(JavaType arg) { return arg; }
    };

    template <> class TypeConversion<double> : public TypeConversionBase_
    {
      public:
        using JavaType = jdouble;
        using NativeType = double;

        static std::string getJavaSignature()
        {
            static std::string sig("D");
            return sig;
        }

        static JavaType nativeToJava(NativeType arg, std::list<Reference> &createdJavaObjects) { return arg; }

        static NativeType takeOwnershipOfJavaValueAndConvertToNative(JavaType arg) { return arg; }
    };

    template <> class TypeConversion<float> : public TypeConversionBase_
    {
      public:
        using JavaType = jfloat;
        using NativeType = float;

        static std::string getJavaSignature()
        {
            static std::string sig("F");
            return sig;
        }

        static JavaType nativeToJava(NativeType arg, std::list<Reference> &createdJavaObjects) { return arg; }

        static NativeType takeOwnershipOfJavaValueAndConvertToNative(JavaType arg) { return arg; }
    };

    template <> class TypeConversion<int64_t> : public TypeConversionBase_
    {
      public:
        using JavaType = jlong;
        using NativeType = int64_t;

        static std::string getJavaSignature()
        {
            static std::string sig("J");
            return sig;
        }

        static JavaType nativeToJava(NativeType arg, std::list<Reference> &createdJavaObjects) { return arg; }

        static NativeType takeOwnershipOfJavaValueAndConvertToNative(JavaType arg) { return arg; }
    };

    template <> class TypeConversion<char32_t> : public TypeConversionBase_
    {
      public:
        using JavaType = jchar;
        using NativeType = char32_t;

        static std::string getJavaSignature()
        {
            static std::string sig("C");
            return sig;
        }

        static JavaType nativeToJava(NativeType arg, std::list<Reference> &createdJavaObjects) { return (jchar)arg; }

        static NativeType takeOwnershipOfJavaValueAndConvertToNative(JavaType arg) { return arg; }
    };

    template <> class TypeConversion<int8_t> : public TypeConversionBase_
    {
      public:
        using JavaType = jbyte;
        using NativeType = int8_t;

        static std::string getJavaSignature()
        {
            static std::string sig("B");
            return sig;
        }

        static JavaType nativeToJava(NativeType arg, std::list<Reference> &createdJavaObjects) { return arg; }

        static NativeType takeOwnershipOfJavaValueAndConvertToNative(JavaType arg) { return arg; }
    };

    template <> class TypeConversion<bool> : public TypeConversionBase_
    {
      public:
        using JavaType = jboolean;
        using NativeType = bool;

        static std::string getJavaSignature()
        {
            static std::string sig("Z");
            return sig;
        }

        static JavaType nativeToJava(NativeType arg, std::list<Reference> &createdJavaObjects)
        {
            return (jboolean)(arg ? JNI_TRUE : JNI_FALSE);
        }

        static NativeType takeOwnershipOfJavaValueAndConvertToNative(JavaType arg) { return arg != JNI_FALSE; }
    };

    template <> class TypeConversion<void> : public TypeConversionBase_
    {
      public:
        using JavaType = void;
        using NativeType = void;

        static std::string getJavaSignature()
        {
            static std::string s("V");
            return s;
        }

        static JavaType nativeToJava() {}

        static NativeType takeOwnershipOfJavaValueAndConvertToNative() {}
    };

    /** Convenience function that converts a C++ native value to its
     * corresponding Java value. See TypeConversion class template for more
     * information.
     *
     *  If a new Java object is created then a reference to it is
     *  stored in the createdJavaObjects list. The object references stored
     * in the list must be kept alive as long as the returned java value is
     * needed. Afterwards they can simply be deleted.
     *
     * */
    template <class NativeType>
    typename TypeConversion<NativeType>::JavaType nativeToJava(NativeType nativeValue,
                                                               std::list<Reference> &createdJavaObjects)
    {
        // To make sure that our specializations work in all cases, we have
        // to strip reference and const qualifiers from the type with
        // std::decay
        return TypeConversion<NativeType>::nativeToJava(static_cast<typename std::decay<NativeType>::type>(nativeValue),
                                                        createdJavaObjects);
    }

    template <class NativeType>
    NativeType takeOwnershipOfJavaValueAndConvertToNative(typename TypeConversion<NativeType>::JavaType javaValue)
    {
        return TypeConversion<NativeType>::takeOwnershipOfJavaValueAndConvertToNative(javaValue);
    }
}
