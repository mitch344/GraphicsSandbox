#pragma once

#include "./Code/Lights/DirectionalLight.h"
#include "./Code/Lights/PointLight.h"
#include "./Code/Lights/Spotlight.h"
#include "./Code/Lights/AreaLight.h"

#include "./Dependencies/imgui-1.90.1/imgui_impl_dx11.h"
#include "./Dependencies/imgui-1.90.1/imgui_impl_win32.h"
#include "./Dependencies/imgui-1.90.1/imgui.h"

class LightUI
{
public:
	static void DrawDirectionalLightUI(DirectionalLight* directionalLight)
	{
		ImGui::Begin("Directional Light");

		//Position
		ImGui::SliderFloat3("Light Position", &directionalLight->mCBDirectionalLightData.position.x, -60.0f, 60.0f);

        // Intensity
        ImGui::SliderFloat("Ambient Intensity", &directionalLight->mCBDirectionalLightData.ambientIntensity, 0.0f, 1.0f);

		//Alpha
		ImGui::SliderFloat("Ambient R", &directionalLight->mCBDirectionalLightData.ambient.x, 0.0f, 1.0f);
		ImGui::SliderFloat("Ambient G", &directionalLight->mCBDirectionalLightData.ambient.y, 0.0f, 1.0f);
		ImGui::SliderFloat("Ambient B", &directionalLight->mCBDirectionalLightData.ambient.z, 0.0f, 1.0f);

		// Diffuse
		ImGui::SliderFloat("Diffuse R", &directionalLight->mCBDirectionalLightData.diffuse.x, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse G", &directionalLight->mCBDirectionalLightData.diffuse.y, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse B", &directionalLight->mCBDirectionalLightData.diffuse.z, 0.0f, 1.0f);

		// Specular
		ImGui::SliderFloat("Specular R", &directionalLight->mCBDirectionalLightData.specular.x, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular G", &directionalLight->mCBDirectionalLightData.specular.y, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular B", &directionalLight->mCBDirectionalLightData.specular.z, 0.0f, 1.0f);
		ImGui::End();
	}

    static void DrawPointLightUI(PointLight* pointLight)
    {
        ImGui::Begin("Point Light");

        // Intensity
        ImGui::SliderFloat("Ambient Intensity", &pointLight->mCBPointLightData.ambientIntensity, 0.0f, 1.0f);

        // Color
        ImGui::ColorEdit3("Ambient Color", &pointLight->mCBPointLightData.ambient.x);
        ImGui::ColorEdit3("Diffuse Color", &pointLight->mCBPointLightData.diffuse.x);
        ImGui::ColorEdit3("Specular Color", &pointLight->mCBPointLightData.specular.x);

        // Position
        ImGui::SliderFloat3("Position", &pointLight->mCBPointLightData.position.x, -60.0f, 60.0f);

        // Additional properties
        ImGui::SliderFloat("Attenuation", &pointLight->mCBPointLightData.attenuation, 0.0f, 10.0f);

        ImGui::End();
    }

    static void DrawSpotLightUI(SpotLight* spotLight)
    {
        ImGui::Begin("Spot Light");

        // Intensity
        ImGui::SliderFloat("Ambient Intensity", &spotLight->mCBSpotLightData.ambientIntensity, 0.0f, 1.0f);

        // Color
        ImGui::ColorEdit3("Ambient Color", &spotLight->mCBSpotLightData.ambient.x);
        ImGui::ColorEdit3("Diffuse Color", &spotLight->mCBSpotLightData.diffuse.x);
        ImGui::ColorEdit3("Specular Color", &spotLight->mCBSpotLightData.specular.x);

        // Position
        ImGui::SliderFloat3("Position", &spotLight->mCBSpotLightData.position.x, -60.0f, 60.0f);

        // Direction
        ImGui::SliderFloat3("Direction", &spotLight->mCBSpotLightData.direction.x, -1.0f, 1.0f);

        // Additional properties
        ImGui::SliderFloat("Attenuation", &spotLight->mCBSpotLightData.attenuation, 0.0f, 10.0f);
        ImGui::SliderFloat("Cone Angle", &spotLight->mCBSpotLightData.coneAngle, 1.0f, 180.0f);

        ImGui::End();
    }

    static void DrawAreaLightUI(AreaLight* areaLight)
    {
        ImGui::Begin("Area Light");

        // Intensity
        ImGui::SliderFloat("Ambient Intensity", &areaLight->mCBAreaLightData.ambientIntensity, 0.0f, 1.0f);

        // Color
        ImGui::ColorEdit3("Ambient Color", &areaLight->mCBAreaLightData.ambient.x);
        ImGui::ColorEdit3("Diffuse Color", &areaLight->mCBAreaLightData.diffuse.x);
        ImGui::ColorEdit3("Specular Color", &areaLight->mCBAreaLightData.specular.x);

        // Position
        ImGui::SliderFloat3("Position", &areaLight->mCBAreaLightData.position.x, -60.0f, 60.0f);

        // Orientation
        ImGui::SliderFloat3("Right", &areaLight->mCBAreaLightData.right.x, -1.0f, 1.0f);
        ImGui::SliderFloat3("Up", &areaLight->mCBAreaLightData.up.x, -1.0f, 1.0f);

        // Additional properties
        ImGui::SliderFloat("Attenuation", &areaLight->mCBAreaLightData.attenuation, 0.0f, 10.0f);
        ImGui::SliderFloat("Width", &areaLight->mCBAreaLightData.width, 1.0f, 100.0f);
        ImGui::SliderFloat("Height", &areaLight->mCBAreaLightData.height, 1.0f, 100.0f);

        ImGui::End();
    }
};