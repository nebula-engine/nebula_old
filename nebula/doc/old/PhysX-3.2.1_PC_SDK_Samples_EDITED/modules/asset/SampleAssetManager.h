
#ifndef SAMPLE_ASSET_MANAGER_H
#define SAMPLE_ASSET_MANAGER_H

#include <Asset.h>
#include <SampleTextureAsset.h>
#include <stdio.h>
#include <vector>

#include <RendererConfig.h>


namespace SampleRenderer
{
	class Renderer;
}

namespace Framework
{
	class Asset;

	class AssetManager
	{
	public:
									AssetManager(SampleRenderer::Renderer &renderer);
									~AssetManager();

		Renderer::Renderer&			getRenderer()								{ return m_renderer; }

		Asset*						getAsset(const char* path, Asset::Type type);
		void						returnAsset(Asset& asset);


		inline const char*			getAssetPathPrefix(void)			const	{ return m_assetPathPrefix; }
	protected:
		Asset*						findAsset(const char* path);
		Asset*						loadAsset(const char* path);
		void						releaseAsset(Asset& asset);

		Asset*						loadXMLAsset(FILE& file, const char* path);
		Asset*						loadTextureAsset(FILE& file, const char* path, SampleTextureAsset::Type texType);
		Asset*						loadODSAsset(FILE& file, const char* path);

	private:
		AssetManager&				operator=(const AssetManager&)				{ return *this; }

	protected:
		Renderer::Renderer&			m_renderer;
		std::vector<Asset*>			m_assets;

		char						m_assetPathPrefix[256];

	};

} // namespace Framework

#endif


