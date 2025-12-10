#pragma once

#include "main.h"
#include "extra_math.h"

using namespace std;

class Input;

namespace util {

    /// @brief Reads the entire content of a text file and returns it as a string.
    /// @param filePath The path to the text file.
    /// @return A string containing the content of the file.
    string loadFileContentToString(const string& filePath);


    template <typename T>
    class array {
        int Capacity = 0;
        int CollectionSize = 0;
        T* Collection = nullptr;

        const int resizeMultiplier = 4;

        void resize() {
            Capacity = (Capacity == 0) ? 1 : Capacity * resizeMultiplier;
            T* CollectionTemp = new T[Capacity];

            for (int i = 0; i < CollectionSize; i++) {
                CollectionTemp[i] = std::move(Collection[i]);
            }

            delete[] Collection;
            Collection = CollectionTemp;
        }

    public:
        array() {
            Capacity = 1;
            Collection = new T[Capacity];
        }

        array(const array& other) {
            Capacity = other.Capacity;
            CollectionSize = other.CollectionSize;
            Collection = new T[Capacity];
            for (int i = 0; i < CollectionSize; i++) {
                Collection[i] = other.Collection[i];
            }
        }

        array& operator=(const array& other) {
            if (this != &other) {
                delete[] Collection;
                Capacity = other.Capacity;
                CollectionSize = other.CollectionSize;
                Collection = new T[Capacity];
                for (int i = 0; i < CollectionSize; i++) {
                    Collection[i] = other.Collection[i];
                }
            }
            return *this;
        }

        ~array() {
            delete[] Collection;
        }

        void append(const T& value) {
            if (Capacity == CollectionSize) {
                resize();
            }
            Collection[CollectionSize] = value;
            CollectionSize++;
        }

        T& operator[](int index) {
            if (index < 0 || index >= CollectionSize)
                throw std::out_of_range("operator[]: index out of range");
            return Collection[index];
        }

        const T& operator[](int index) const {
            if (index < 0 || index >= CollectionSize)
                throw std::out_of_range("operator[]: index out of range");
            return Collection[index];
        }

        T& get_ref(int index) {
            if (index < 0 || index >= CollectionSize)
                throw std::out_of_range("GetRef: index out of range");
            return Collection[index];
        }

        const T& get_ref(int index) const {
            if (index < 0 || index >= CollectionSize)
                throw std::out_of_range("GetRef: index out of range");
            return Collection[index];
        }

        T* data() const {
            if (CollectionSize == 0) return nullptr;

            T* copy = new T[CollectionSize];
            for (int i = 0; i < CollectionSize; i++) {
                copy[i] = Collection[i];
            }
            return copy;
        }

        void remove(int index) {
            if (index >= 0 && index < CollectionSize) {
                for (int i = index; i < CollectionSize - 1; i++) {
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

        void reserve(int capacity) {
            if (capacity > Capacity) {
                T* newCollection = new T[capacity];
                for (int i = 0; i < CollectionSize; i++) {
                    newCollection[i] = std::move(Collection[i]);
                }
                delete[] Collection;
                Collection = newCollection;
                Capacity = capacity;
            }
        }
    };

    struct face_index {
        int v;   // vertex index (1-based if matching OBJ, or convert to 0-based)
        int vt;  // uv index, 0 if missing
        int vn;  // normal index, 0 if missing
    };

    struct dynamic_mesh_data {
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
        
        static static_mesh loadObj(const string& filePath);
        static array<string> parseObjFaceData(const string& face_data);
        static void parseFaceDataFaster(const string& line, dynamic_mesh_data& m);
        static void triangulate_face(const array<unsigned int>& indices, array<unsigned int>& face_indices);
        static bool valid_index(int index, int size);
        static void insert_item_in_array(int start_index, array<unsigned int>& indices, const array<string>& face_indices, int size);
    };


    class mesh_collection {
        map<string, static_mesh> static_mesh_holder;
    public:
        mesh_collection();
        ~mesh_collection();

        void loadStaticMeshToHolder(const string& name, const char* path);
        static_mesh& getStaticMesh(const string& name);
    };

    class texture_data {
        GLuint id = 0;
        int width = 0;
        int height = 0;
        bool dataLoaded = false;
    public:
        texture_data() = default;
        ~texture_data();

        bool loadTexture(const char* path);
        GLuint getID() const;
        int getWidth() const;
        int getHeight() const;
        bool isLoaded() const;
    };

    class textures_data_collection {
        map<string, texture_data> textures;
    public:
        textures_data_collection();
        ~textures_data_collection();

        void loadTextureToHolder(const string& name, const char* path);
        texture_data& getTexture(const string& name);
    };
    
    extern textures_data_collection textures_data_collection_instance;
    extern mesh_collection mesh_collection_instance;
    extern Input* input;
}