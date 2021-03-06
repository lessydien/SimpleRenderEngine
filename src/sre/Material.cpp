/*
 *  SimpleRenderEngine (https://github.com/mortennobel/SimpleRenderEngine)
 *
 *  Created by Morten Nobel-Jørgensen ( http://www.nobel-joergensen.com/ )
 *  License: MIT
 */

#include "sre/Material.hpp"
#include "sre/impl/GL.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/color_space.hpp>
#include "sre/Renderer.hpp"
#include "sre/Log.hpp"


namespace sre {

    Material::Material(std::shared_ptr<Shader> shader)
    :shader{nullptr}
    {
        setShader(std::move(shader));
        name = "Undefined material";
    }

    Material::~Material(){
    }

    void Material::bind(){
        unsigned int textureSlot = 0;
        for (const auto & t : textureValues) {
            glActiveTexture(GL_TEXTURE0 + textureSlot);
            glBindTexture(t.value->target, t.value->textureId);
            glUniform1i(t.id, textureSlot);
            textureSlot++;
        }
        for (auto& t : vectorValues) {
            glUniform4fv(t.id, 1, glm::value_ptr(t.value));
        }
        for (auto& t : floatValues) {
            glUniform1f(t.id, t.value);
        }
        for (auto& t : mat3Values) {
            if (t.value.get()) {
                glUniformMatrix3fv(t.id, static_cast<GLsizei>(t.value->size()), GL_FALSE, glm::value_ptr((*t.value)[0]));
            }
        }
        for (auto& t : mat4Values) {
            if (t.value.get()){
                glUniformMatrix4fv(t.id, static_cast<GLsizei>(t.value->size()), GL_FALSE, glm::value_ptr((*t.value)[0]));
            }
        }
    }

    std::shared_ptr<sre::Shader> Material::getShader()  {
        return shader;
    }

    void Material::setShader(std::shared_ptr<sre::Shader> shader) {
        Material::shader = shader;

        textureValues.clear();
        vectorValues.clear();
        floatValues.clear();

        for (auto & u : shader->uniforms){
            switch (u.type){
                case UniformType::Vec4:
                {
                    Uniform<glm::vec4> uniform;
                    uniform.id = u.id;
                    uniform.value = glm::vec4(1.0f,1.0f,1.0f,1.0f);
                    vectorValues.push_back(uniform);
                }
                break;
                case UniformType::Texture:
                {
                    Uniform<std::shared_ptr<sre::Texture>> uniform;
                    uniform.id = u.id;
                    uniform.value = Texture::getWhiteTexture();
                    textureValues.push_back(uniform);
                }
                break;
                case UniformType::TextureCube:
                {
                    Uniform<std::shared_ptr<sre::Texture>> uniform;
                    uniform.id = u.id;
                    uniform.value = Texture::getDefaultCubemapTexture();
                    textureValues.push_back(uniform);
                }
                break;
                case UniformType::Float:
                {
                    Uniform<float> uniform;
                    uniform.id = u.id;
                    uniform.value = 0.0f;
                    floatValues.push_back(uniform);
                }
                break;
                case UniformType::Mat3:
                {
                    Uniform<std::shared_ptr<std::vector<glm::mat3>>> uniform;
                    uniform.id = u.id;
                    mat3Values.push_back(uniform);
                }
                break;
                case UniformType::Mat4:
                {
                    Uniform<std::shared_ptr<std::vector<glm::mat4>>> uniform;
                    uniform.id = u.id;
                    mat4Values.push_back(uniform);
                }
                break;
                default:
                    LOG_ERROR("'%s' Unsupported uniform type: %i. Only Vec4, Texture, TextureCube and Float is supported.", u.name.c_str(), (int)u.type);
                    break;
            }
        }
    }

    Color Material::getColor()   {
        return get<Color>("color");
    }

    bool Material::setColor(const Color &color) {
        return set("color", color);
    }

    Color Material::getSpecularity()   {
        return get<Color>("specularity");
    }

    bool Material::setSpecularity(Color specularity) {
        return set("specularity", specularity);
    }

    std::shared_ptr<sre::Texture> Material::getTexture()  {
        return get<std::shared_ptr<sre::Texture>>("tex");
    }

    bool Material::setTexture(std::shared_ptr<sre::Texture> texture) {
        return set("tex",texture);
    }

    const std::string &Material::getName()  {
        return name;
    }

    void Material::setName(const std::string &name) {
        Material::name = name;
    }

    bool Material::set(std::string uniformName, glm::vec4 value){
        auto type = shader->getUniformType(uniformName);
        for (auto & v : vectorValues){
            if (v.id==type.id){
                v.value = value;
                return true;
            }
        }
        return false;
    }

    bool Material::set(std::string uniformName, std::shared_ptr<std::vector<glm::mat3>> value){
        auto type = shader->getUniformType(uniformName);
        for (auto & v : mat3Values){
            if (v.id==type.id){
                v.value = value;
                return true;
            }
        }
        return false;
    }

    bool Material::set(std::string uniformName, std::shared_ptr<std::vector<glm::mat4>> value){
        auto type = shader->getUniformType(uniformName);
        for (auto & v : mat4Values){
            if (v.id==type.id){
                v.value = value;
                return true;
            }
        }
        return false;
    }

    bool Material::set(std::string uniformName, Color value){
        auto type = shader->getUniformType(uniformName);
        for (auto & v : vectorValues){
            if (v.id==type.id){
                v.value = value.toLinear();
                return true;
            }
        }
        return false;
    }



    bool Material::set(std::string uniformName, float value){
        auto type = shader->getUniformType(uniformName);
        for (auto & v : floatValues){
            if (v.id==type.id){
                v.value = value;
                return true;
            }
        }
        return false;
    }

    bool Material::set(std::string uniformName, std::shared_ptr<sre::Texture> value){
        auto type = shader->getUniformType(uniformName);
        for (auto & v : textureValues){
            if (v.id==type.id){
                v.value = value;
                return true;
            }
        }
        return false;
    }

    std::shared_ptr<sre::Texture> Material::getMetallicRoughnessTexture() {
        return get<std::shared_ptr<sre::Texture>>("mrTex");
    }

    bool Material::setMetallicRoughnessTexture(std::shared_ptr<sre::Texture> texture) {
        return set("mrTex",texture);

    }

    glm::vec2 Material::getMetallicRoughness() {
        return (glm::vec2)get<glm::vec4>("metallicRoughness");
    }

    bool Material::setMetallicRoughness(glm::vec2 metallicRoughness) {
        return set("metallicRoughness",glm::vec4(metallicRoughness,0,0));
    }
}