/// \file material.h
/// \brief Header file for V-ART class "Material".
/// \version $Revision: 1.5 $

#ifndef VART_MATERIAL_H
#define VART_MATERIAL_H

#include "vart/texture.h"
#include "vart/color.h"

namespace VART {
/// \class Material material.h
/// \brief Material properties for graphical objects.
///
/// FixMe: Put detailed description here.
    class Material {
        public:
        // Public Methods
            Material();

            /// \brief Creates a new material, creating a copy of another one.
            Material(const Material& m);

            /// Creates plastic-looking material of given color.
            Material(const Color& c);

            /// \brief Creates a Material, aplying a texture to it.
            Material(const Texture& t);

            /// Sets all colors to be shades of one single color.
            Material(const Color& c, float spc, float amb, float ems, float shi);

            /// \brief Copies all texture data from one to another.
            Material& operator=(const Material& m);

            /// \brief Makes the material to have a plastic-looking of given color.

            /// Sets the diffuse, specular, ambient and emissive colors of the material,
            /// to make it to have a plastic-looking of a given color.
            void SetPlasticColor(const VART::Color& c);

            /// Sets the diffuse color (main color) of the material.
            void SetDiffuseColor(const Color& c) { color = c; }

            /// Returns the diffuse color.
            const Color& GetDiffuseColor() const { return color; }

            /// Sets the specular color (highlight color) of the material.
            void SetSpecularColor(const Color& c) { specular = c; }

            /// Returns the specular color of the material.
            const Color& GetSpecularColor() const { return specular; }

            /// Sets the ambient color of the material.
            void SetAmbientColor(const Color& c) { ambient = c; }

            /// Returns the ambient color of the material.
            const Color& GetAmbientColor() const { return ambient; }

            /// Sets the emissive color of the material.
            void SetEmissiveColor(const Color& c) { emissive = c; }

            /// Returns the emissive color of the material.
            const Color& GetEmissiveColor() const { return emissive; }

            /// Sets the texture of material.
            void SetTexture(const Texture& t);

            /// \brief Indicates if a material contains texture.
            bool HasTexture() const { return texture.HasData(); }

            /// \brief Returns the current texture of material.
            const VART::Texture& GetTexture() const { return texture; }

            /// \brief Set the shininess of the material.

            /// According to the "OpenGL programming guide" (red book), shininess should be
            /// in the range of 0.0 (no shininess) to 128.0 (very shiny).
            void SetShininess(float newValue) { shininess = newValue; }

            /// \brief Return the shininess of the material.
            float GetShininess() const { return shininess; }

            /// \brief Draws the material using OpenGL engine.
            /// \return false if V-ART has not been compiled with OpenGL support.
            bool DrawOGL() const;

        // Public Static Methods
            static const Material& LIGHT_PLASTIC_GRAY();
            static const Material& DARK_PLASTIC_GRAY();
            static const Material& PLASTIC_WHITE();
            static const Material& PLASTIC_RED();
            static const Material& PLASTIC_GREEN();
            static const Material& PLASTIC_BLUE();
            static const Material& PLASTIC_BLACK();
    private:
            /// color for diffuse reflection
            Color color;
            /// color for light emission
            Color emissive;
            /// color for ambient light reflection
            Color ambient;
            /// color for specular light reflection
            Color specular;
            /// texture data
            Texture texture;
            /// shininess coeficient
            float shininess;
    }; // end class declaration
} // end namespace

#endif
