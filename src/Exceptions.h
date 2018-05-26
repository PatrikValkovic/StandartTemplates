#ifndef TEMPLATES_EXCEPTIONS_H
#define TEMPLATES_EXCEPTIONS_H


namespace Templates
{
    class Exception
    {
    protected:
        const char *Message;
        int LineNumber;

        virtual const char* ToString() const
        {
            // return os << "Error in " << this->Message << " on line " << this->LineNumber << std::endl;
			return this->Message;
        }

    public:
        Exception(const char *Message, const int &LineNumber)
        {
            this->Message = Message;
            this->LineNumber = LineNumber;
        };

        virtual ~Exception()
        { }
    };

    class InvalidArgumentException : public Exception
    {
    public:
        InvalidArgumentException(const char *Message, int LineNumber)
                : Exception(Message, LineNumber)
        { }

        virtual ~InvalidArgumentException()
        { }
    };

    class VectorException : public Exception
    {
    public:
        VectorException(const char *Message, int LineNumber)
                : Exception(Message, LineNumber)
        { }

        virtual ~VectorException()
        { }
    };

    class OutOfRangeException : public Exception
    {
    public:
        OutOfRangeException() : Exception("Index is out of range.", -1)
        { }

        OutOfRangeException(const char *Message)
                : Exception(Message, -1)
        { }

        OutOfRangeException(const char *Message, int LineNumber)
                : Exception(Message, LineNumber)
        { }

        virtual ~OutOfRangeException()
        { }
    };

    class QueueException : public Exception
    {
    public:
        QueueException(const char *File, const int Line) :
                Exception(File, Line)
        { }

        virtual ~QueueException()
        { }
    };

    class ListException : public Exception
    {
    public:
        ListException(const char *File, const int Line) :
                Exception(File, Line)
        { }

        virtual ~ListException()
        { }
    };

    class ArrayException : public Exception
    {
    public:
        ArrayException(const char *File, const int Line) :
                Exception(File, Line)
        { }

        virtual ~ArrayException()
        { }
    };

    class NotImplementedException : public Exception
    {
    public:
        NotImplementedException(const char *File, const int Line) :
                Exception(File, Line)
        { }

        virtual ~NotImplementedException()
        { }
    };

    class InternalException : public Exception
    {
    public:
        InternalException() : Exception("Index is out of range.", -1)
        { }

        InternalException(const char *Message)
                : Exception(Message, -1)
        { }

        InternalException(const char *Message, int LineNumber)
                : Exception(Message, LineNumber)
        { }

        virtual ~InternalException()
        { }
    };
}

namespace Math
{
    class IntervalException : public Templates::Exception
    {
    public:
        IntervalException(const char *File, const int Line) :
                Exception(File, Line)
        { }

        virtual ~IntervalException()
        { }
    };

}

#endif //TEMPLATES_EXCEPTIONS_H
