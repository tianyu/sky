#ifndef TEST_ARGUMENT_PASSING
#define TEST_ARGUMENT_PASSING

struct DataObject
{
    DataObject() :
        copies(0),
        moves(0),
        data(0),
        _valid(true)
    {}

    DataObject(int data) :
        copies(0),
        moves(0),
        data(data),
        _valid(true)
    {}

    DataObject(DataObject const&o) :
        copies(o.copies + 1),
        moves(o.moves),
        data(o.data),
        _valid(o._valid)
    {}

    DataObject(DataObject &&o) :
        copies(o.copies),
        moves(o.moves + 1),
        data(o.data),
        _valid(o._valid)
    {
        o._valid = false;
    }

    bool valid() const
    {
        return _valid;
    }

    const int copies, moves;
    int data;

private:
    bool _valid;
};

#endif // TEST_ARGUMENT_PASSING
