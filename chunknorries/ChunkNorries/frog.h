#pragma once

#include <iterator>
#include <memory>
#include <iostream>

namespace chucknorries {
    template<typename T>
    class Allocator {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using const_pointer = const T *;
        using reference = T &;
        using const_reference = const T &;

        Allocator()

        noexcept {}

        Allocator(const Allocator<T> &other)

        noexcept =
        default;

        template<class U>
        Allocator(const Allocator<U> &other)

        noexcept {}

        ~Allocator() = default;

        Allocator<T> &operator=(const Allocator<T> &) = default;

        pointer allocate(size_type n) {
            pointer p = static_cast<pointer>(malloc(sizeof(value_type) * n));
            if (p)
                return p;

            throw std::bad_alloc();
        }


        void deallocate(pointer p, size_type n)

        noexcept {
            (void) n;
            free(p);
        }
    };


    template<typename ValueType>
    class ChunkListIterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType *;
        using reference = ValueType &;
        using size_type = std::size_t;

    private:
        pointer value = nullptr;
        ChunkListIterator <value_type> *chunk = nullptr;
        size_type index = 0;

    public:
        ChunkListIterator()

        noexcept =
        default;

        ChunkListIterator(pointer current_value, size_type current_index, ChunkListIterator <value_type> *current_chunk) :
                value(current_value), index(current_index), chunk(current_chunk) {}

        ChunkListIterator(const ChunkListIterator &other)

        noexcept =
        default;

        ChunkListIterator &operator=(const ChunkListIterator &other) = default;

        ~ChunkListIterator() = default;

        size_type GetIndex() const {
            return index;
        }

        friend void swap(ChunkListIterator<ValueType> &first, ChunkListIterator<ValueType> &second) {
            std::swap(first.chunk, second.chunk);
            std::swap(first.value, second.value);
            std::swap(first.index, second.index);
        }

        friend bool operator==(const ChunkListIterator<ValueType> &first,
                               const ChunkListIterator<ValueType> &second) {
            return first.value == second.value;
        }

        friend bool operator!=(const ChunkListIterator<ValueType> &first,
                               const ChunkListIterator<ValueType> &second) {
            return first.value != second.value;
        }

        reference operator*() const {
            return *this->value;
        }

        pointer operator->() const {
            return this->value;
        }

        ChunkListIterator &operator++() {
            if  (this->index + 1 == this->chunk->GetSize()) {
                this->chunk = nullptr;
                this->value = nullptr;
                this->index = 0;
            } else {
                this->value = &this->chunk->At(++this->index);
            }
            return *this;
        }

        ChunkListIterator operator++(int) {
            ChunkListIterator temp = *this;
            ++(*this);
            return temp;
        }

        ChunkListIterator &operator--() {
            if (this->index == 0) {
                throw std::out_of_range("Index is out of range.");
            }
            this->value = &this->chunk->At(--this->index);
            return *this;
        }

        ChunkListIterator operator--(int) {
            ChunkListIterator temp = *this;
            --(*this);
            return temp;
        }

        ChunkListIterator operator+(difference_type difference) const {
            return ChunkListIterator(&this->chunk->At(this->index + difference), this->index + difference, this->chunk);
        }

        ChunkListIterator &operator+=(difference_type difference) {
            this->index += difference;
            this->value = &this->chunk->At(this->index);
            return *this;
        }

        ChunkListIterator operator-(difference_type difference) const {
            return ChunkListIterator(&this->chunk->At(this->index - difference), this->index - difference, this->chunk);
        }

        ChunkListIterator &operator-=(difference_type difference) {
            this->index -= difference;
            this->value = &this->chunk->At(this->index);
            return *this;
        }

        reference operator[](difference_type n) const {
            return this->chunk->At(n);
        }

        friend bool operator<(const ChunkListIterator<ValueType> &first,
                              const ChunkListIterator<ValueType> &second) {
            return first.index < second.index;
        }

        friend bool operator<=(const ChunkListIterator<ValueType> &first,
                               const ChunkListIterator<ValueType> &second) {
            return first.index <= second.index;
        }

        friend bool operator>(const ChunkListIterator<ValueType> &first,
                              const ChunkListIterator<ValueType> &second) {
            return first.index > second.index;
        }

        friend bool operator>=(const ChunkListIterator<ValueType> &first,
                               const ChunkListIterator<ValueType> &second) {
            return first.index >= second.index;
        }
    };


    template<typename ValueType>
    class ChunkList_const_iterator : public ChunkListIterator<ValueType> {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = const ValueType *;
        using reference = const ValueType &;

        ChunkList_const_iterator() : ChunkList_iterator<value_type>() {};

        ChunkList_const_iterator(value_type *value, int index, IChunkList <value_type> *chunk) :
                ChunkList_iterator<ValueType>(value, index, chunk) {};

        ChunkList_const_iterator(const ChunkList_const_iterator &other)

        noexcept =
        default;

        ChunkList_const_iterator(pointer value, int index, const IChunkList <value_type> *chunk) :
                ChunkList_iterator<value_type>(const_cast<value_type *>(value), index,
                                               const_cast<IChunkList <value_type> *>(chunk)) {}

        ChunkList_const_iterator &operator=(const ChunkList_const_iterator &) = default;

        ~ChunkList_const_iterator() = default;

        friend void swap(ChunkList_const_iterator<ValueType> &first,
                         ChunkList_const_iterator<ValueType> &second) {
            std::swap(first.value, second.value);
            std::swap(first.chunk, second.chunk);
            std::swap(first.index, second.index);
        }

        int GetIndex() {
            return index;
        }

        friend bool operator==(const ChunkList_const_iterator<ValueType> &first,
                               const ChunkList_const_iterator<ValueType> &second) {
            return first.value == second.value;
        }

        friend bool operator!=(const ChunkList_const_iterator<ValueType> &first,
                               const ChunkList_const_iterator<ValueType> &second) {
            return first.value != second.value;
        }

        reference operator*() const {
            return *this->value;
        }

        pointer operator->() const {
            return this->value;
        }

        ChunkList_const_iterator &operator++() {
            if (index + 1 == chunk->GetSize()) {
                this->chunk = nullptr;
                this->value = nullptr;
                this->index = 0;
                return *this;
            }
            this->value = &chunk->At(++index);
            return *this;
        }

        ChunkList_const_iterator operator++(int) {
            if (index + 1 == chunk->GetSize()) {
                return ChunkList_const_iterator();
            }
            this->value = &chunk->At(++index);
            return *this;
        }

        ChunkList_const_iterator &operator--() {
            if (index - 1 < 0) {
                throw std::exception();
            }
            this->value = &chunk->At(--index);
            return *this;
        }

        ChunkList_const_iterator operator--(int) {
            if (index - 1 < 0) {
                throw std::exception();
            }
            this->value = &chunk->At(--index);
            return *this;
        }

        ChunkList_const_iterator operator+(const difference_type &difference) const {
            return ChunkList_const_iterator(&chunk->At(index + difference), index + difference, chunk);
        }

        ChunkList_const_iterator &operator+=(const difference_type &difference) {
            this->index += difference;
            this->value = &chunk->At(index);
            return *this;
        }

        ChunkList_const_iterator operator-(const difference_type &difference) const {
            return ChunkList_const_iterator(&chunk->At(index - difference), index - difference, chunk);
        }

        ChunkList_const_iterator &operator-=(const difference_type &difference) {
            this->index -= difference;
            this->value = &chunk->At(index);
            return *this;
        }

        reference operator[](const difference_type &n) {
            return chunk[n];
        }

        friend bool operator<(const ChunkList_const_iterator<ValueType> &first,
                              const ChunkList_const_iterator<ValueType> &second) {
            return first.index < second.index;
        }

        friend bool operator<=(const ChunkList_const_iterator<ValueType> &first,
                               const ChunkList_const_iterator<ValueType> &second) {
            return first.index <= second.index;
        }

        friend bool operator>(const ChunkList_const_iterator<ValueType> &first,
                              const ChunkList_const_iterator<ValueType> &second) {
            return first.index > second.index;
        }

        friend bool operator>=(const ChunkList_const_iterator<ValueType> &first,
                               const ChunkList_const_iterator<ValueType> &second) {
            return first.index >= second.index;
        }
    };

    template<typename ValueType>
    class IChunkList {
    public:
        virtual std::size_t GetSize() const

        noexcept = 0;

        virtual ValueType &At(std::size_t position) = 0;

        virtual ValueType &operator[](std::ptrdiff_t position) = 0;

        virtual ~IChunkList() = default;
    };

    template<typename ValueType>
    class Chunk : public IChunkList<ValueType> {
    public:
        using reference = ValueType &;
        using size_type = std::size_t;
        using pointer = ValueType *;

    private:
        pointer list = nullptr;
        size_type current_size = 0;
        Allocator<ValueType> allocator;

    public:
        explicit Chunk(size_type chunk_size, const Allocator<ValueType> &custom_allocator = Allocator<ValueType>())
                : list(allocator.allocate(chunk_size)), current_size(0) {}

        ~Chunk() {
            allocator.deallocate(list, current_size);
        }

        size_type GetSize() const

        noexcept override{
                return current_size;
        }

        reference At(size_type position) override {
            if (position >= current_size) {
                throw std::out_of_range("Position is out of range!");
            }
            return list[position];
        }

        reference operator[](std::ptrdiff_t position) override {
            return list[position];
        }

        pointer CopyElements() {
            pointer values = allocator.allocate(current_size);
            std::uninitialized_copy(list, list + current_size, values);
            return values;
        }
    };

    template<typename T, std::size_t N, typename Allocator = Allocator<T>>
    class ChunkList : public IChunkList<T> {
    public:
        using value_type = T;
        using allocator_type = Allocator;
        using size_type = std::size_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        using iterator = ChunkListIterator<value_type>;
        using const_iterator = ChunkListConstIterator<value_type>;

    private:
        std::size_t size = 0;
        Chunk<value_type> *start = nullptr;
        allocator_type allocator;

    public:
        explicit ChunkList(const allocator_type &custom_allocator = allocator_type())
                : allocator(custom_allocator), size(0), start(nullptr) {}

        ~ChunkList() {
            Clear();
        }

        std::size_t GetSize() const

        noexcept override{
                return size;
        }

        reference At(size_type position) override {
            Chunk<value_type> *current_chunk = FindChunk(position);
            return current_chunk->At(position % N);
        }

        reference operator[](std::ptrdiff_t position) override {
            return At(static_cast<size_type>(position));
        }

        void Clear() {
            Chunk<value_type> *current_chunk = start;
            while (current_chunk) {
                Chunk<value_type> *next_chunk = current_chunk->next;
                delete current_chunk;
                current_chunk = next_chunk;
            }
            size = 0;
            start = nullptr;
        }

        void PushBack(const value_type &value) {
            if (size % N == 0) {
                AddChunk();
            }
            Chunk<value_type> *last_chunk = FindChunk(size);
            last_chunk->At(size % N) = value;
            ++size;
        }

        iterator begin()

        noexcept {
            return iterator(start, size, this);
        }

        iterator end()

        noexcept {
            return iterator(nullptr, size, this);
        }

        const_iterator cbegin() const

        noexcept {
            return const_iterator(start, size, this);
        }

        const_iterator cend() const

        noexcept {
            return const_iterator(nullptr, size, this);
        }

    private:
        Chunk<value_type> *FindChunk(size_type position) const {
            if (position >= size) {
                throw std::out_of_range("Position is out of range!");
            }

            Chunk<value_type> *current_chunk = start;
            while (current_chunk && position >= current_chunk->GetSize()) {
                position -= current_chunk->GetSize();
                current_chunk = current_chunk->next;
            }

            return current_chunk;
        }

        void AddChunk() {
            Chunk<value_type> *new_chunk = new Chunk<value_type>(N, allocator);
            if (!start) {
                start = new_chunk;
            } else {
                Chunk<value_type> *last_chunk = start;
                while (last_chunk->next) {
                    last_chunk = last_chunk->next;
                }
                last_chunk->next = new_chunk;
                new_chunk->prev = last_chunk;
            }
        }
    };


    public:

        ChunkList() : start(new Chunk<value_type>(N)) {}

        explicit ChunkList(const Allocator &alloc) : start(new Chunk<value_type>(N, alloc)) {}

        size_t GetSize() const

        noexcept override{
            return size;
        }

        // Конструктор с явным указанием количества элементов и аллокатора по умолчанию
        explicit ChunkList(size_type
            count,
            const Allocator &alloc = Allocator()
            ) :

            start (nullptr), size(0) {
                Chunk<value_type> *temp_pointer = nullptr;
                Chunk<value_type> *pointer_prev = nullptr;

                for (size_type i = 0; i < count; ++i) {
                    if (temp_pointer == nullptr || temp_pointer->current_size == N) {
                        if (temp_pointer != nullptr) {
                            pointer_prev = temp_pointer;
                        }
                        temp_pointer = new Chunk<value_type>(N, alloc);
                        if (pointer_prev != nullptr) {
                            pointer_prev->next = temp_pointer;
                            temp_pointer->prev = pointer_prev;
                        }
                    }

                    temp_pointer->allocator = alloc;
                    temp_pointer->list[temp_pointer->current_size++] = value_type();
                    ++size;
                }
            }

        // Конструктор копирования
        ChunkList(const ChunkList &other) : start(nullptr), size(0) {
                Chunk<value_type> *other_list = other.start;
                Chunk<value_type> *this_list = nullptr;

                while (other_list != nullptr) {
                    if (this_list == nullptr) {
                        this_list = new Chunk<value_type>(N);
                        start = this_list;
                    } else {
                        this_list->next = new Chunk<value_type>(N);
                        this_list->next->prev = this_list;
                        this_list = this_list->next;
                    }

                    this_list->current_size = other_list->current_size;
                    this_list->list = other_list->CopyElements();
                    size += this_list->current_size;

                    other_list = other_list->next;
                }
        }

        // Конструктор копирования с указанием аллокатора
        ChunkList(const ChunkList &other, const Allocator &alloc) : start(nullptr), size(0) {
            Chunk<value_type> *other_list = other.start;
            Chunk<value_type> *this_list = nullptr;

            while (other_list != nullptr) {
                if (this_list == nullptr) {
                    this_list = new Chunk<value_type>(N, alloc);
                    start = this_list;
                } else {
                    this_list->next = new Chunk<value_type>(N, alloc);
                    this_list->next->prev = this_list;
                    this_list = this_list->next;
                }

                this_list->current_size = other_list->current_size;
                this_list->list = other_list->CopyElements();
                size += this_list->current_size;

                other_list = other_list->next;
            }
        }


        // Конструктор копирования с указанием аллокатора
        ChunkList(const ChunkList &other, const Allocator &alloc)
                : start(new Chunk<value_type>(N, alloc)) {
            Chunk<value_type> *other_list = other.start;
            Chunk<value_type> *this_list = start;
            size = other.size;

            while (other_list != nullptr) {
                this_list->current_size = other_list->current_size;
                this_list->list = other_list->CopyElements();
                this_list->next = new Chunk<value_type>(N, alloc);
                this_list->prev = this_list;

                other_list = other_list->next;
                if (other_list != nullptr) {
                    this_list = this_list->next;
                }
            }
        }

        // Конструктор перемещения
            ChunkList(ChunkList &&other) noexcept
            : size(std::exchange(other.size, 0)), start(std::exchange(other.start, nullptr)) {
            other.clear();
        }

        // Конструктор перемещения с указанием аллокатора
        ChunkList(ChunkList &&other, const Allocator &alloc)
        : ChunkList(std::move(other)) {
            for (Chunk<value_type> *temp_pointer = start; temp_pointer != nullptr; temp_pointer = temp_pointer->next) {
                temp_pointer->allocator = alloc;
            }
        }

        // Деструктор
        ~ChunkList() {
            clear();
        }

        // Оператор присваивания копирования
        ChunkList &operator=(const ChunkList &other) {
            return *this = ChunkList(other);
        }

        // Оператор присваивания перемещения
        ChunkList &operator=(ChunkList &&other) noexcept {
        size = std::exchange(other.size, 0);
        start = std::exchange(other.start, nullptr);
        other.clear();
        return *this;
        }

        // Метод assign
        void assign(size_type count, const T &value) {
            if (count > 0) {
                clear();
                for (int i = 0; i < count; i++) {
                    push_back(value);
                }
                size = count;
            }
        }

        // Метод get_allocator
        allocator_type get_allocator() const noexcept {
            return start->allocator;
        }

        // Метод At
        reference At(size_type pos) override {
            if (pos < 0 || pos >= max_size()) {
                throw std::out_of_range("Position is out of range!");
            }
            int chunk_number = pos / N;
            int value_number = pos % N;
            Chunk<value_type> *temp_pointer = start;
            for (int i = 0; i < chunk_number; i++) {
                temp_pointer = temp_pointer->next;
            }
            return temp_pointer->At(value_number);
        }

        // Метод At (константная версия)
        const_reference At(size_type pos) const override {
            if (pos < 0 || pos >= max_size()) {
                throw std::out_of_range("Position is out of range!");
            }
            int chunk_number = pos / N;
            int value_number = pos % N;
            Chunk<value_type> *temp_pointer = start;
            for (int i = 0; i < chunk_number; i++) {
            temp_pointer = temp_pointer->next;
            }
            return const_cast<reference>(temp_pointer->At(value_number));
        }

        // Оператор доступа по индексу
        reference operator[](difference_type pos) override {
            int chunk_number = pos / N;
            int value_number = pos % N;
            Chunk<value_type> *temp_pointer = start;
            for (int i = 0; i < chunk_number; i++) {
                temp_pointer = temp_pointer->next;
            }
            return temp_pointer->list[value_number];
        }

        // Оператор доступа по индексу (константная версия)
        const_reference operator[](difference_type pos) const override {
            int chunk_number = pos / N;
            int value_number = pos % N;
            Chunk<value_type> *temp_pointer = start;
            for (int i = 0; i < chunk_number; i++) {
                temp_pointer = temp_pointer->next;
            }
            return const_cast<reference>(temp_pointer->list[value_number]);
        }

        // Метод front
        reference front() {
            if (size > 0)
                return start->list[0];
            else
                throw std::runtime_error("ChunkList is empty!");
        }

        // Метод front (константная версия)
        const_reference front() const {
            if (size > 0) {
                return static_cast<reference>(start->list[0]);
            } else {
                throw std::runtime_error("ChunkList is empty!");
            }
        }

        // Метод back
        reference back() {
            if (size == 0) {
                throw std::runtime_error("ChunkList is empty!");
            }
            Chunk<value_type> *temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            return temp_pointer->list[temp_pointer->current_size - 1];
        }

        // Метод back (константная версия)
        const_reference back() const {
            if (size == 0) {
                throw std::runtime_error("ChunkList is empty!");
            }
            Chunk<value_type> *temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            return const_cast<reference>(temp_pointer->list[temp_pointer->current_size - 1]);
        }


        reference front() {
            if (size > 0)
                return start->list[0];
            else
                throw std::runtime_error("ChunkList is empty!");
        }

        const_reference front() const {
            if (size > 0) {
                static_cast<reference>(start->list[0]);
            } else throw std::runtime_error("ChunkList is empty!");
        }

        reference back() {
            if (size == 0) {
                throw std::runtime_error("ChunkList is empty!");
            }
            Chunk <value_type> *temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            return temp_pointer->list[temp_pointer->current_size - 1];
        }

        const_reference back() const {
            if (size == 0) {
                throw std::runtime_error("ChunkList is empty!");
            }
            Chunk <value_type> *temp_pointer = start;
            while (temp_pointer->next != nullptr) {
                temp_pointer = temp_pointer->next;
            }
            return const_cast<reference>(temp_pointer->list[temp_pointer->current_size - 1]);
        }

        // Метод begin
        iterator begin()noexcept {
        return (size == 0) ? end() : ChunkList_iterator<value_type>(&At(0), 0, this);
        }

        // Константный метод begin
        const_iterator begin() const noexcept {
        return (size == 0) ? end() : ChunkList_const_iterator<value_type>(&At(0), 0, this);
        }

        // Константный метод cbegin
        const_iterator cbegin() const noexcept {
        return begin();
        }

        // Метод end
        iterator end() noexcept {
        return ChunkList_iterator<value_type>();
        }

        // Константный метод end
        const_iterator end() const noexcept {
        return ChunkList_const_iterator<value_type>();
        }

        // Константный метод cend
        const_iterator cend() const noexcept {
        return end();

        }

        // Метод empty
        bool empty() const noexcept {
        return size == 0;
        }

        // Метод get_size
        int get_size() const noexcept {
        return size;
        }

        // Метод max_size
        size_type max_size() const noexcept {
        size_type value_number = size % N;
        return (value_number == 0) ? size : size + N - value_number;
        }

        // Метод clear
        void clear() noexcept {
        Chunk <value_type> *current_chunk = start;
        while (current_chunk != nullptr) {
        Chunk <value_type> *temp_pointer = current_chunk;
        current_chunk = current_chunk->next;
        delete temp_pointer;
        }
        start = nullptr;
        size = 0;
        }

        iterator insert(const_iterator pos, const T &value) {
            if (pos == cend())
                return end();

            int index = pos.GetIndex();
            Chunk<value_type> *temp_chunk = FindLastChunk();
            ChunkList_iterator<value_type> iterator(&temp_chunk->
                    list[temp_chunk->current_size - 1], size - 1, this);

            if (size < max_size()) {
                ShiftElementsRight(iterator, pos, size);
                size++;
            } else {
                AddNewChunkAfter(temp_chunk);
                temp_chunk = temp_chunk->next;
                size++;
                ShiftElementsRight(iterator, pos, size);
            }

            At(index) = value;
            temp_chunk->current_size++;
            return ChunkList_iterator<value_type>(&At(index), index, this);
        }

        // функция для нахождения последнего чанка
        Chunk<value_type>* FindLastChunk() {
            Chunk<value_type> *temp_chunk = start;
            while (temp_chunk->next != nullptr) {
                temp_chunk = temp_chunk->next;
            }
            return temp_chunk;
        }

        // функция для сдвига элементов вправо
        void ShiftElementsRight(ChunkList_iterator<value_type> &iterator, const_iterator pos, size_t &new_size) {
            for (int i = 0; iterator >= pos; i++, iterator--) {
                At(new_size - i) = At(new_size - i - 1);
            }
        }

        iterator insert(const_iterator pos, T &&value) {
            if (pos == cend())
                return end();

            int index = pos.GetIndex();
            Chunk<value_type> *last_chunk = FindLastChunk();

            ChunkList_iterator<value_type> iterator(&last_chunk->
                    list[last_chunk->current_size - 1], size - 1, this);

            if (size < max_size()) {
                ShiftElementsRight(iterator, pos, size);
                size++;
            } else {
                AddNewChunkAfter(last_chunk);
                last_chunk = last_chunk->next;
                size++;
                ShiftElementsRight(iterator, pos, size);
            }

            At(index) = std::move(value);
            last_chunk->current_size++;
            return ChunkList_iterator<value_type>(&At(index), index, this);
        }

        iterator erase(const_iterator pos) {

            int index = pos.GetIndex();

            if (index + 1 == size) {
                size--;
                return ChunkList_iterator<value_type>();
            }

            ShiftElementsLeft(index, size);
            Chunk<value_type> *last_chunk = FindLastChunk();

            if (size == max_size() - N + 1) {
                last_chunk = last_chunk->prev;
                delete last_chunk->next;
                last_chunk->next = nullptr;
            } else {
                last_chunk->current_size--;
            }

            size--;
            return ChunkList_iterator<value_type>(&At(index), index, this);
        }

        // Функция ShiftElementsLeft
        void ShiftElementsLeft(int start, int end) {
            for (int i = start; i < end - 1; i++) {
                At(i) = At(i + 1);
            }
        }

        iterator erase(const_iterator first, const_iterator last) {
            int start_index = first.GetIndex();
            int end_index = last.GetIndex();
            auto iterator = begin() + start_index;

            for (int i = 0; iterator != last; ++iterator, ++i) {
                At(start_index + i) = std::move(At(end_index + i));
            }

            size -= (end_index - start_index);

            // Находим последний чанк после изменений
            Chunk<value_type> *last_chunk = FindLastChunk();

            while (end_index > 0) {
                if (last_chunk->current_size > 0) {
                    last_chunk->current_size--;
                    --end_index;
                } else {
                    last_chunk = last_chunk->prev;
                    delete last_chunk->next;
                    last_chunk->next = nullptr;
                }
            }

            return ChunkList_iterator<value_type>(&At(start_index), start_index, this);
        }

        void push_back(const T &value) {
            EnsureNotEmptyChunk();
            Chunk<value_type> *last_chunk = FindLastChunkOrCreateNew();

            if (last_chunk->current_size == last_chunk->size) {
                last_chunk = AppendNewChunk();
            }

            last_chunk->list[last_chunk->current_size++] = value;
            size++;
        }

        void push_back(T &&value) {
            EnsureNotEmptyChunk();
            Chunk<value_type> *last_chunk = FindLastChunkOrCreateNew();

            if (last_chunk->current_size == last_chunk->size) {
                last_chunk = AppendNewChunk();
            }

            last_chunk->list[last_chunk->current_size++] = std::move(value);
            size++;
        }

        void pop_back() {
            if (start == nullptr || size == 0) {
                throw std::runtime_error("ChunkList is empty!");
            }

            Chunk<value_type> *last_chunk = FindLastChunk();
            last_chunk->current_size--;
            size--;
            RemoveEmptyChunks();
        }

        void push_front(const T &value) {
            insert(cbegin(), value);
        }

        void push_front(T &&value) {
            insert(cbegin(), std::move(value));
        }

        void pop_front() {
            erase(cbegin());
        }

        void swap(ChunkList &other) {
            std::swap(start, other.start);
            std::swap(size, other.size);
        }

        friend bool operator==(const ChunkList &lhs, const ChunkList &rhs) {
            if (lhs.size != rhs.size) {
                return false;
            }

            auto lhs_iterator = lhs.begin();
            auto rhs_iterator = rhs.begin();

            while (lhs_iterator != lhs.end()) {
                if (*lhs_iterator != *rhs_iterator) {
                    return false;
                }
                ++lhs_iterator;
                ++rhs_iterator;
            }

            return true;
        }

        template <class U, int N, class Alloc>
        friend bool operator!=(const ChunkList<U, N, Alloc> &lhs, const ChunkList<U, N, Alloc> &rhs) {
            return !(lhs == rhs);
        }
    };
}