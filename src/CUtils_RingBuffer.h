#pragma once

#include <stdint.h>

// Исходник отсюда: http://we.easyelectronics.ru/Soft/kolcevoy-bufer-na-s-dlya-mk.html
// Исходник существенно переработан.

typedef uint16_t RingBufferIndex_T;

template <RingBufferIndex_T SIZE, typename DATA_T = uint8_t>
class RingBuffer
{
    static_assert(SIZE > 0, "Ring buffer should be > 0");
    static_assert((SIZE & (SIZE - 1)) == 0, "Ring buffer should be power of two");

private:
    DATA_T _data[SIZE];

    // данные пишутся в "голову" (head) и читаются из "хвоста" (tail)
    volatile RingBufferIndex_T _head;
    volatile RingBufferIndex_T _tail;

    static constexpr RingBufferIndex_T _mask = SIZE - 1;
    inline static constexpr RingBufferIndex_T nextIndex(RingBufferIndex_T current_index) noexcept
    {
        return (current_index + 1) & _mask;
    };

public:
    // запись в буфер, возвращает true если значение записано
    inline bool Write(DATA_T value) noexcept
    {
        if (IsFull())
            return false;

        _data[_head] = value;
        _head = nextIndex(_head);

        return true;
    }

    // чтение из буфера, возвращает true если значение прочитано
    inline bool Read(DATA_T &value) noexcept
    {
        if (IsEmpty())
            return false;

        value = _data[_tail];
        _tail = nextIndex(_tail);

        return true;
    }

    // пуст ли буфер
    inline bool IsEmpty() const noexcept
    {
        return _head == _tail;
    }

    // полон ли буфер
    inline bool IsFull() const noexcept
    {
        return nextIndex(_head) == _tail;
    }

    // количество элементов в буфере
    inline RingBufferIndex_T Count() const noexcept
    {
        return (_head - _tail) & _mask;
    }

    // очистить буфер
    inline void Clear() const noexcept
    {
        _tail = 0;
        _head = 0;
    }

    // размер буфера
    inline static constexpr RingBufferIndex_T Size() noexcept
    {
        return SIZE;
    }
};
