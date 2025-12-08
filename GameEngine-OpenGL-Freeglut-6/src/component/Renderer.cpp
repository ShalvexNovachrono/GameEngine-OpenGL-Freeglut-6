#include "../../include/component/Renderer.h"

#include "../../include/GameObject.h"
#include "../../include/component/Transform.h"

void Renderer::start() {
    base_component::start();
}

void Renderer::update() {
    base_component::update();

}

void Renderer::display() {
    glEnable(GL_DEPTH_TEST);
    if (isTextureLoaded) {
        glBindTexture(GL_TEXTURE_2D, texture.getID());
    }

    glEnableClientState(GL_VERTEX_ARRAY);

    if (isTextureLoaded) { glEnableClientState(GL_TEXTURE_COORD_ARRAY); }
    if (mesh.normals) { glEnableClientState(GL_NORMAL_ARRAY); }

    glVertexPointer(3, GL_FLOAT, sizeof(Vec3), mesh.vertices);

    if (isTextureLoaded) { glTexCoordPointer(2, GL_FLOAT, sizeof(Vec2), mesh.uvs); }

    if (mesh.normals) { glNormalPointer(GL_FLOAT, sizeof(Vec3), mesh.normals); }


    glPushMatrix();

    if (!isTextureLoaded) {
        glColor4f(color.x, color.y, color.z, color.w);
    } else {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);  
    }

    glScalef(gameObject->transform->scale.x, gameObject->transform->scale.y, gameObject->transform->scale.z);
    glTranslatef(gameObject->transform->position.x, gameObject->transform->position.y, gameObject->transform->position.z);
    glRotatef(gameObject->transform->rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(gameObject->transform->rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(gameObject->transform->rotation.z, 0.0f, 0.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, mesh.v_vertex_indices_count, GL_UNSIGNED_INT, mesh.v_vertex_indices);

    for (auto& callback : callbacks) {
        callback();
    }

    glPopMatrix();

    if (isTextureLoaded) { glDisableClientState(GL_TEXTURE_COORD_ARRAY); }
    
   
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Renderer::setMesh(const string& meshName) {
    mesh = util::mesh_holder_instance.getStaticMesh(meshName);
    isMeshLoaded = true;
}

void Renderer::setTexture(const string& textureName) {
    texture = util::textures_holder_instance.getTexture(textureName);
    isTextureLoaded = true;
}

Renderer* Renderer::clone() {
    return new Renderer(*this);
}
