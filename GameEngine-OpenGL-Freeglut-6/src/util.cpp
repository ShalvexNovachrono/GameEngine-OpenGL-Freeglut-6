#define STB_IMAGE_IMPLEMENTATION  
#include "../include/util.h"
#include "../include/stb_image.h"


namespace util {
    string read_file_as_string(const string& file_path) {
        ifstream file(file_path);
        if (!file.is_open()) {
            LOG_ERROR("read_file_as_string: Could not open file: " + file_path);
        }

        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }


    array<string> parse_obj_face_data(string face_data) {
        istringstream face_data_stream(face_data); // with out the first "f " characters
        array<string> n_words = array<string>();
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

    static_mesh load_obj(const string& file_path) {
        string file_content = read_file_as_string(file_path);

        istringstream file_stream(file_content);

        string line;

        int vertex_count = 0;
        int uv_count = 0;
        int normal_count = 0;
        int face_count = 0;
        int face_vertex_count = 0;
        int face_uv_count = 0;
        int face_normal_count = 0;


        while (getline(file_stream, line)) {
            size_t char_space_pos = static_cast<size_t>((line.find_first_of(' ') != string::npos) ? line.find_first_of(' ') + 1 : 0);
            string parsed_line = line.substr(0, char_space_pos - 1);
            if (parsed_line == "v") {
                vertex_count++;
            }
            else if (parsed_line == "vt") {
                uv_count++;
            }
            else if (parsed_line == "vn") {
                normal_count++;
            }
            else if (parsed_line == "f") {
                face_count++;

                string face_data_parse_1 = line.substr(char_space_pos, line.size());

                array<string> face_data_parse_2 = parse_obj_face_data(face_data_parse_1);

                for (int i = 0; i < 9; i += 3) {
                    if (face_data_parse_2[i] != "") face_vertex_count++;
                    if (face_data_parse_2[i + 1] != "") face_uv_count++;
                    if (face_data_parse_2[i + 2] != "") face_normal_count++;
                }
            }
        }

        file_stream.clear();
        file_stream.seekg(0, ios::beg); // Reset stream to beginning

        mesh m;

        while (getline(file_stream, line)) {
            if (line.size() < 2) continue; // Skip empty or too short lines

            istringstream line_stream(line);
            string prefix;
            line_stream >> prefix;

            if (prefix == "v") {
                Vec3 vertex;
                try {
                    line_stream >> vertex.x >> vertex.y >> vertex.z;
                }
                catch (...) {
                    LOG_ERROR("load_obj: Invalid vertex data.");
                }
                m.vertices.append(vertex);
            } else if (prefix == "vt") {
                Vec2 uv;
                try {
                    line_stream >> uv.x >> uv.y;
                }
                catch (...) {
                    LOG_ERROR("load_obj: Invalid UV data.");
                }
                m.uvs.append(uv);
            } else if (prefix == "vn") {
                Vec3 normal;
                try {
                    line_stream >> normal.x >> normal.y >> normal.z;
                }
                catch (...) {
                    LOG_ERROR("load_obj: Invalid normal data.");
                }
                m.normals.append(normal);
            } else if (prefix == "f") {
                string face_data_line = line.substr(2, line.size());
                array<string> paresed_face_data = parse_obj_face_data(face_data_line);

                array<unsigned int> vertex_indices = array<unsigned int>();
                array<unsigned int> uv_indices = array<unsigned int>();
                array<unsigned int> normal_indices = array<unsigned int>();

                // 1/2/3 4/5/6 7/8/9

                /*
                v_idx_1 = (paresed_face_data[0] != "") ? stoi(paresed_face_data[0]) - 1 : -1;
                v_idx_2 = (paresed_face_data[3] != "") ? stoi(paresed_face_data[3]) - 1 : -1;
                v_idx_3 = (paresed_face_data[6] != "") ? stoi(paresed_face_data[6]) - 1 : -1;

                if (v_idx_1 == -1 || v_idx_2 == -1 || v_idx_3 == -1) {
                    LOG_ERROR("load_obj: Face vertex index missing or invalid.");
                }

                if (!valid_index(v_idx_1, m.vertices.size()) || !valid_index(v_idx_2, m.vertices.size()) || !valid_index(v_idx_3, m.vertices.size())) {
                    LOG_ERROR("load_obj: Face vertex index out of range.");
                }

                vertex_indices.append(v_idx_1);
                vertex_indices.append(v_idx_2);
                vertex_indices.append(v_idx_3);
                */

                insert_item_in_array(0, vertex_indices, paresed_face_data, m.vertices.size());
                if (m.uvs.size() > 0)
                    insert_item_in_array(1, uv_indices, paresed_face_data, m.uvs.size());

                if (m.normals.size() > 0)
                    insert_item_in_array(2, normal_indices, paresed_face_data, m.normals.size());

                triangulate_face(vertex_indices, m.v_vertex_indices);
                triangulate_face(uv_indices, m.vt_uv_indices);
                triangulate_face(normal_indices, m.vn_normal_indices);
            }
        }

        static_mesh m_static;
        m_static.vertices = m.vertices.copy_as_pointer();
        m_static.uvs = m.uvs.copy_as_pointer();
        m_static.normals = m.normals.copy_as_pointer();
        m_static.faces = m.faces.copy_as_pointer();

        m_static.vertex_count = vertex_count;
        m_static.uv_count = uv_count;
        m_static.normal_count = normal_count;
        m_static.face_count = face_count;

        m_static.v_vertex_indices = m.v_vertex_indices.copy_as_pointer();
        m_static.vt_uv_indices = m.vt_uv_indices.copy_as_pointer();
        m_static.vn_normal_indices = m.vn_normal_indices.copy_as_pointer();

        m_static.v_vertex_indices_count = m.v_vertex_indices.size();
        m_static.vt_uv_indices_count = m.vt_uv_indices.size();
        m_static.vn_normal_indices_count = m.vn_normal_indices.size();


        LOG_DEBUG("load_obj: Loaded " + to_string(vertex_count) + " vertices, " + to_string(uv_count) + " uvs, " + to_string(normal_count) + " normals, " + to_string(face_count) + " faces from " + file_path);
        return m_static;
    }

    void triangulate_face(const array<unsigned int>& indices, array<unsigned int>& face_indicess) {
        if (indices.size() < 3) return; // Not enough vertices to form a face
        for (int i = 1; i < indices.size() - 1; ++i) {
            face_indicess.append(indices[0]);
            face_indicess.append(indices[i]);
            face_indicess.append(indices[i + 1]);
        }
    }

    bool valid_index(int index, int size) {
        return index >= 0 && index < size;
    }

    void insert_item_in_array(int start_index, array<unsigned int>& indices, const array<string>& face_indices, int size) {
        const int a = start_index;     // 0, 1, or 2
        const int b = a + 3;           // 3, 4, or 5
        const int c = a + 6;           // 6, 7, or 8

        int idx_1 = (face_indices[a] != "") ? stoi(face_indices[a]) - 1 : -1;
        int idx_2 = (face_indices[b] != "") ? stoi(face_indices[b]) - 1 : -1;
        int idx_3 = (face_indices[c] != "") ? stoi(face_indices[c]) - 1 : -1;

        if (idx_1 == -1 || idx_2 == -1 || idx_3 == -1)
            LOG_ERROR("load_obj: index missing or invalid.");

        if (!valid_index(idx_1, size) || !valid_index(idx_2, size) || !valid_index(idx_3, size)) {
            LOG_DEBUG("Index values: " + to_string(idx_1) + ", " + to_string(idx_2) + ", " + to_string(idx_3) + " | Size: " + to_string(size));
            LOG_ERROR("load_obj: index out of range.");
        }

        indices.append(idx_1);
        indices.append(idx_2);
        indices.append(idx_3);
    }

    mesh_holder::mesh_holder() {
        static_mesh_holder = map<string, static_mesh>();
    }

    mesh_holder::~mesh_holder() {
        static_mesh_holder.clear();
    }

    void mesh_holder::loadStaticMeshToHolder(const string& name, const char* path) {
        static_mesh_holder[name] = load_obj(path);
    }

    static_mesh& mesh_holder::getStaticMesh(const string& name) {
        auto it = static_mesh_holder.find(name);
        if (it == static_mesh_holder.end()) {
            throw out_of_range("Static mesh not found: " + name);
        }
        return it->second;
    }


    texture_data::texture_data() {
    }

    texture_data::~texture_data() {
        if (dataLoaded && id != 0) {
            glDeleteTextures(1, &id);
        }
    }

    bool texture_data::loadTexture(const char* path) {
        BLog_Debug("Attempting to load texture from path: " << path);

        // Load image using stb_image
        int channels;
        unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

        if (!data) {
            BLog_WARNING("Failed to load texture: " << path);
            BLog_WARNING("Reason: " << stbi_failure_reason());
            return false;
        }

        // Generate OpenGL texture
        glGenTextures(1, &id);
        if (id == 0) {
            BLog_WARNING("Failed to generate OpenGL texture ID.");
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
            BLog_WARNING("Unexpected number of channels: " << channels);
            format = GL_RGB; // Fallback to RGB
            break;
        }

        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // Check for OpenGL errors during texture upload
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            BLog_ERROR("OpenGL Error during texture upload: " << error);
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

        BLog_Debug("Texture loaded successfully: " << path);
        BLog_Debug("Dimensions: " << width << "x" << height << ", Channels: " << channels);
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

    textures_holder::textures_holder() {
        textures = map<string, texture_data>();
    }

    textures_holder::~textures_holder() {
        textures.clear();
    }

    void textures_holder::loadTextureToHolder(const string& name, const char* path) {
        textures[name] = texture_data();
        textures[name].loadTexture(path);
    }

    texture_data& textures_holder::getTexture(const string& name) {
        auto it = textures.find(name);
        if (it == textures.end()) {
            throw out_of_range("Texture not found: " + name);
        }
        return it->second;
    }

    
    textures_holder textures_holder_instance;
    mesh_holder mesh_holder_instance;
}
