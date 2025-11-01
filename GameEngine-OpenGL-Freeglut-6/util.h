#pragma once

#include "main.h"
#include "extra_math.h"

using namespace std;

namespace util {

    /// @brief Reads the entire content of a text file and returns it as a string.
    /// @param filePath The path to the text file.
    /// @return A string containing the content of the file.
    static string read_file_as_string(const string& filePath);


    template <typename T>
    class array {
    private:
        int Capacity = 0;
        int CollectionSize = 0;
        T* Collection = nullptr;

        void resize() {
            Capacity = (Capacity == 0) ? 1 : Capacity * 10;
            T* CollectionTemp = new T[Capacity];

            for (int i = 0; i < CollectionSize; i++) {
                CollectionTemp[i] = Collection[i];
            }

            delete[] Collection;
            Collection = CollectionTemp;
        }

    public:
        array() {
            Capacity = 1;
            Collection = new T[Capacity];
        }

        array(const array& Other) {
            Capacity = Other.Capacity;
            CollectionSize = Other.CollectionSize;
            Collection = new T[Capacity];
            for (int i = 0; i < CollectionSize; i++) {
                Collection[i] = Other.Collection[i];
            }
        }

        array& operator=(const array& Other) {
            if (this != &Other) {
                delete[] Collection;
                Capacity = Other.Capacity;
                CollectionSize = Other.CollectionSize;
                Collection = new T[Capacity];
                for (int i = 0; i < CollectionSize; i++) {
                    Collection[i] = Other.Collection[i];
                }
            }
            return *this;
        }

        ~array() {
            delete[] Collection;
        }

        void append(T Value) {
            if (Capacity == CollectionSize) {
                resize();
            }
            Collection[CollectionSize] = Value;
            CollectionSize++;
        }

        T& operator[](int Index) {
            if (Index < 0 || Index >= CollectionSize)
                throw std::out_of_range("operator[]: index out of range");
            return Collection[Index];
        }

        const T& operator[](int Index) const {
            if (Index < 0 || Index >= CollectionSize)
                throw std::out_of_range("operator[]: index out of range");
            return Collection[Index];
        }

        T& get_ref(int idx) {
            if (idx < 0 || idx >= CollectionSize)
                throw std::out_of_range("GetRef: index out of range");
            return Collection[idx];
        }

        const T& get_ref(int idx) const {
            if (idx < 0 || idx >= CollectionSize)
                throw std::out_of_range("GetRef: index out of range");
            return Collection[idx];
        }

        T* copy_as_pointer() const {
            if (CollectionSize == 0) return nullptr;

            T* copy = new T[CollectionSize];
            for (int i = 0; i < CollectionSize; i++) {
                copy[i] = Collection[i];
            }
            return copy;
        }

        void remove(int Index) {
            if (Index >= 0 && Index < CollectionSize) {
                for (int i = Index; i < CollectionSize - 1; i++) {
                    Collection[i] = Collection[i + 1];
                }
                CollectionSize--;
            }
        }

        int size() const {
            return CollectionSize;
        }

        bool empty() const {
            return CollectionSize == 0;
        }

        int capacity() const {
            return Capacity;
        }

        void clear() {
            CollectionSize = 0;
        }
    };

    struct face_index {
        int v;   // vertex index (1-based if matching OBJ, or convert to 0-based)
        int vt;  // uv index, 0 if missing
        int vn;  // normal index, 0 if missing
    };

    struct mesh {
        array<Vec3> vertices;
        array<Vec2> uvs;
        array<Vec3> normals;

        // Face data
        array<unsigned int> v_vertex_indices;
        array<unsigned int> vt_uv_indices;
        array<unsigned int> vn_normal_indices;


        array<face_index> faces;
    };

    struct static_mesh {
        Vec3* vertices = nullptr;
        Vec2* uvs = nullptr;
        Vec3* normals = nullptr;
        face_index* faces = nullptr;

        int vertex_count = 0;
        int uv_count = 0;
        int normal_count = 0;
        int face_count = 0;

        unsigned int* v_vertex_indices = nullptr;
        unsigned int* vt_uv_indices = nullptr;
        unsigned int* vn_normal_indices = nullptr;

        int v_vertex_indices_count = 0;
        int vt_uv_indices_count = 0;
        int vn_normal_indices_count = 0;
    };


    array<string> parse_obj_face_data(string face_data);

    static_mesh load_obj(const string& file_path);

    void triangulate_face(const array<unsigned int>& indices, array<unsigned int>& face_indicess);

    bool valid_index(int index, int size);

    void insert_item_in_array(int start_index, array<unsigned int>& indices, const array<string>& face_indices, int size);
}