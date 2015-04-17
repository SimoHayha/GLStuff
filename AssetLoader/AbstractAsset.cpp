#include "AssetHelper.h"
#include "AbstractAsset.h"

AbstractAsset::AbstractAsset(std::string const& file, std::string const& path) :
m_guid(AssetHelper::NewUUID()),
m_name(file),
m_path(path),
m_dirty(true)
{
}

AbstractAsset::~AbstractAsset()
{
}

bool AbstractAsset::Load()
{
	return OnLoad();
}

void AbstractAsset::OnUnload()
{
	OnUnload();
}

bool AbstractAsset::Serialize()
{
	return OnSerialize();
}

std::string AbstractAsset::GetCompletePath() const
{
	return (m_path + "\\" + m_name);
}

bool AbstractAsset::IsDirty() const
{
	return m_dirty;
}

