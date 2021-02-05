#pragma once

#include "utils/types.hpp"
#include "math/vec4.hpp"
#include "math/mat4.hpp"
#include <unordered_map>
#include <vector>

namespace Aery {
    using PGLObject = mut_u32;
    using PGLShader = mut_u32;

    class Window;

    struct GLVertex {
        vec3 position;
        vec3 color;
    };

    struct GLShader {
        mut_u32 program;
        mut_u32 id;
    };

    struct GLObject {
        struct {
            std::vector<GLVertex> list;
            mut_u32 id;
        } vbo;

        struct {
            std::vector<mut_u16> list;
            mut_u32 id;
        } ebo;
        mut_u32 vao;

        struct {
            mat4 transform;
        } push_constant;

        PGLShader shader = 0;
        mut_u32 id;

        bool operator==(const GLObject& Other) {
            return id == Other.id;
        }
    };

    struct GLRendererCreateInfo {
        Window* window = nullptr;
    };

    struct GLShaderCreateInfo {
        const char* vertex;
        const char* fragment;
    };

    struct GLObjectCreateInfo {
        std::vector<GLVertex> vertices;
        std::vector<mut_u16> indices;
    };

    class GLRenderer {
    public:
        GLRenderer(); ~GLRenderer();

        bool create(GLRendererCreateInfo&&) = delete;
        bool create(GLRendererCreateInfo&);
        void destroy();
        void draw();

        // Object methods

        GLObject& getObjectByID(u32);
        bool createDefaultObject(PGLObject*);
        bool createObject(GLObjectCreateInfo&&, PGLObject*) = delete;
        bool createObject(GLObjectCreateInfo&, PGLObject*);
        void destroyObject(PGLObject);

        // Shader methods

        GLShader& getShaderByID(u32);
        bool createDefaultShader(PGLShader*);
        bool createShader(GLShaderCreateInfo&&, PGLShader*) = delete;
        bool createShader(GLShaderCreateInfo&, PGLShader*);
        void destroyShader(PGLShader);

        // Events

        void _onResize();

    private:
        Window* m_Window = nullptr;
        u32 m_ID = -1;
    };
}
