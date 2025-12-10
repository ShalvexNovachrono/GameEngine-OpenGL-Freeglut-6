#define STB_IMAGE_IMPLEMENTATION  
#include "../include/stb_image.h"
#include "../include/util.h"

#include <strstream>

#include "../include/idh.h"



namespace util {
    string loadFileContentToString(const string& file_path) {
        ifstream file(file_path);
        if (!file.is_open()) {
            LOG_ERROR("loadFileContentToString: Could not open file: " + file_path)
        }

        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    static_mesh static_mesh::loadObj(const string& filePath) {
        string fileContent = loadFileContentToString(filePath);

        istringstream fileStream(fileContent);

        string line;

        static_mesh m_static;
        dynamic_mesh_data m;
        
        size_t fileSize = fileContent.size();
        int estimatedVertices = static_cast<int>(fileSize) / 2; // Estimate
        m.vertices.reserve(estimatedVertices);
        m.uvs.reserve(estimatedVertices);
        m.normals.reserve(estimatedVertices);

        while (getline(fileStream, line)) {
            if (line.size() < 2) continue; // Skip empty or too short lines

            char prefix = line[0];

            if (prefix == 'v') {
                if (line[1] == ' ') {
                    m_static.vertex_count++;

                    istringstream lineStream(line.substr(2));
                    
                    Vec3 vertex;
                    try {
                        lineStream >> vertex.x >> vertex.y >> vertex.z;
                    } catch (...) {
                        LOG_WARNING("loadObj: Invalid vertex data.")
                    }
                    m.vertices.append(vertex);
                } else if (line[1] == 't') { // vt
                    m_static.uv_count++;

                    istringstream lineStream(line.substr(3));
                    
                    Vec2 uv;
                    try {
                        lineStream >> uv.x >> uv.y;
                    } catch (...) {
                        LOG_WARNING("loadObj: Invalid UV data.")
                    }
                    m.uvs.append(uv);
                }  else if (line[1] == 'n') { // vn
                    m_static.normal_count++;

                    istringstream lineStream(line.substr(3));
                    
                    Vec3 normal;
                    try {
                        lineStream >> normal.x >> normal.y >> normal.z;
                    } catch (...) {
                        LOG_WARNING("loadObj: Invalid normal data.")
                    }
                    m.normals.append(normal);
                } 
            } else if (prefix == 'f') {
                m_static.face_count++;
                
                parseFaceDataFaster(line, m);
            }
        }

        m_static.vertices = m.vertices.data();
        m_static.uvs = m.uvs.data();
        m_static.normals = m.normals.data();
        m_static.faces = m.faces.data();
        
        m_static.v_vertex_indices = m.v_vertex_indices.data();
        m_static.vt_uv_indices = m.vt_uv_indices.data();
        m_static.vn_normal_indices = m.vn_normal_indices.data();

        m_static.v_vertex_indices_count = m.v_vertex_indices.size();
        m_static.vt_uv_indices_count = m.vt_uv_indices.size();
        m_static.vn_normal_indices_count = m.vn_normal_indices.size();

        LOG_DEBUG("loadObj: Loaded " + to_string(m_static.vertex_count) + " vertices, " + to_string(m_static.uv_count) + " uvs, " + to_string(m_static.normal_count) + " normals, " + to_string(m_static.face_count) + " faces from " + filePath)
        return m_static;
    }

    array<string> static_mesh::parseObjFaceData(const string& face_data) {
        istringstream face_data_stream(face_data); // without the first "f " characters
        array<string> n_words;
        string word;
        while (face_data_stream >> word) {
            istringstream sp(word);
            string sa, sb, sc;
            getline(sp, sa, '/');
            getline(sp, sb, '/');
            getline(sp, sc, '/');

            n_words.append(sa);
            n_words.append(sb);
            n_words.append(sc);
        }
        return n_words;
    }

    void static_mesh::parseFaceDataFaster(const string& line, dynamic_mesh_data& m) {
        const char* ptr = line.c_str() + 2; // Skip "f "
    
        array<unsigned int> vertex_indices;
        array<unsigned int> uv_indices;
        array<unsigned int> normal_indices;
    
        while (*ptr) {
            // Skip whitespace
            while (*ptr == ' ' || *ptr == '\t') ptr++;
            if (!*ptr) break;
        
            // Parse vertex/uv/normal indices directly
            int v = parse_number(ptr);
            int vt = 0, vn = 0;
        
            if (*ptr == '/') {
                ptr++;
                vt = parse_number(ptr);
                if (*ptr == '/') {
                    ptr++;
                    vn = parse_number(ptr);
                }
            }
        
            if (v > 0) vertex_indices.append(v - 1);
            if (vt > 0) uv_indices.append(vt - 1);
            if (vn > 0) normal_indices.append(vn - 1);
        }
    
        triangulate_face(vertex_indices, m.v_vertex_indices);
        triangulate_face(uv_indices, m.vt_uv_indices);
        triangulate_face(normal_indices, m.vn_normal_indices);
    }


    void static_mesh::triangulate_face(const array<unsigned int>& indices, array<unsigned int>& face_indices) {
        if (indices.size() < 3) return; // Not enough vertices to form a face
        for (int i = 1; i < indices.size() - 1; ++i) {
            face_indices.append(indices[0]);
            face_indices.append(indices[i]);
            face_indices.append(indices[i + 1]);
        }
    }

    bool static_mesh::valid_index(int index, int size) {
        return index >= 0 && index < size;
    }

    void static_mesh::insert_item_in_array(int start_index, array<unsigned int>& indices, const array<string>& face_indices, int size) {
        // Loop through all vertices in the face (step by 3 for v/vt/vn structure)
        for (int i = start_index; i < face_indices.size(); i += 3) {
            const string& val = face_indices[i];
            int index = (!val.empty()) ? stoi(val) - 1 : -1;

            if (index == -1)
                LOG_ERROR("loadObj: index missing or invalid.")

            if (!valid_index(index, size)) {
                LOG_DEBUG("Index value: " + to_string(index) + " | Size: " + to_string(size))
                LOG_ERROR("loadObj: index out of range.")
            }

            indices.append(index);
        }
    }

    mesh_collection::mesh_collection() {
        static_mesh_holder = map<string, static_mesh>();
    }

    mesh_collection::~mesh_collection() {
        static_mesh_holder.clear();
    }

    void mesh_collection::loadStaticMeshToHolder(const string& name, const char* path) {
        static_mesh_holder[name] = static_mesh::loadObj(path);
    }

    static_mesh& mesh_collection::getStaticMesh(const string& name) {
        auto it = static_mesh_holder.find(name);
        if (it == static_mesh_holder.end()) {
            throw out_of_range("Static dynamic_mesh_data not found: " + name);
        }
        return it->second;
    }


    texture_data::~texture_data() {
        if (dataLoaded && id != 0) {
            glDeleteTextures(1, &id);
        }
    }

    bool texture_data::loadTexture(const char* path) {
        BLog_Debug("Attempting to load texture from path: " << path)

        // Load image using stb_image
        int channels;
        unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

        if (!data) {
            BLog_WARNING("Failed to load texture: " << path)
            BLog_WARNING("Reason: " << stbi_failure_reason())
            return false;
        }

        // Generate OpenGL texture
        glGenTextures(1, &id);
        if (id == 0) {
            BLog_WARNING("Failed to generate OpenGL texture ID.")
            stbi_image_free(data);
            return false;
        }

        glBindTexture(GL_TEXTURE_2D, id);

        // Determine format based on channels
        GLenum format;
        switch (channels) {
        case 1: format = GL_LUMINANCE; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default:
            BLog_WARNING("Unexpected number of channels: " << channels)
            format = GL_RGB; // Fallback to RGB
            break;
        }

        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // Check for OpenGL errors during texture upload
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            BLog_ERROR("OpenGL Error during texture upload: " << error)
            stbi_image_free(data);
            return false;
        }

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Clean up
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);

        BLog_Debug("Texture loaded successfully: " << path)
        BLog_Debug("Dimensions: " << width << "x" << height << ", Channels: " << channels)
        dataLoaded = true;


        return dataLoaded;
    }

    GLuint texture_data::getID() const {
        return id;
    }

    int texture_data::getWidth() const {
        return width;
    }

    int texture_data::getHeight() const {
        return height;
    }

    bool texture_data::isLoaded() const {
        return dataLoaded;
    }

    textures_data_collection::textures_data_collection() {
        textures = map<string, texture_data>();
    }

    textures_data_collection::~textures_data_collection() {
        textures.clear();
    }

    void textures_data_collection::loadTextureToHolder(const string& name, const char* path) {
        textures[name] = texture_data();
        textures[name].loadTexture(path);
    }

    texture_data& textures_data_collection::getTexture(const string& name) {
        auto it = textures.find(name);
        if (it == textures.end()) {
            throw out_of_range("Texture not found: " + name);
        }
        return it->second;
    }

    
    textures_data_collection textures_data_collection_instance;
    mesh_collection mesh_collection_instance;
    Input* input;
}
