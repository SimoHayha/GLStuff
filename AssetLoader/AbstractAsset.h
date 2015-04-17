#ifndef ABSTRACTASSET_H__
#define ABSTRACTASSET_H__

#include <string>

class AbstractAsset
{
public:
	AbstractAsset(std::string const& file, std::string const& path);
	virtual ~AbstractAsset();

	bool	Load();
	void	Unload();
	bool	Serialize();

	virtual bool OnLoad() = 0;
	virtual void OnUnload() = 0;
	virtual bool OnSerialize() = 0;

	std::string	GetCompletePath() const;
	bool		IsDirty() const;

protected:
	enum AssetType
	{
		TEXTURE,
		UNKNOWN,
		SIZE_OF_ENUM
	};

	std::string	m_guid;
	std::string	m_name;
	std::string m_path;
	bool		m_dirty;
};

#endif