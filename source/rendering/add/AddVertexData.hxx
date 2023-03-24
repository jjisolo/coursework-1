#pragma once

namespace GL::Intf {

    template<size_t VerticiesCount> class AddVertexData {
        public:
            explicit AddVertexData(float vertexData[VerticiesCount]):
                m_fVerticies(vertexData)
            {
            }

            virtual ~AddVertexData();

            inline float* getVerticies() const {
                return m_fVerticies;
            }

            inline void setVerticies(float newVertexData[VerticiesCount]) {
                m_fVerticies = newVertexData;
            }

        protected:
            float m_fVerticies[VerticiesCount];
    };
}