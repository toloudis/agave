/****************************************************************************\
**  envSharedAssetMgr.hpp
**
**      envSharedAssetMgr.hpp defines template for the pattern where
**	a manager keeps track of shared assets using a reference count.
**	Assets are freed when the reference count reaches 0.
**
**	Note: bga - I would prefer that this use shared_ptr in the near future.
**
**
**
\****************************************************************************/
#ifdef ENV_SHAREDASSETMGR_HPP
#error envSharedAssetMgr.hpp multiply included
#endif
#define ENV_SHAREDASSETMGR_HPP

#include <map>
#include <list>

//================================================================================
// Reference counting manager -
// In order to use this class, you should implement a "LoadAsset" function
//	with the same signature as the Load() function.
//================================================================================
template<class AssetClass, class LocatorClass>
class envSharedAssetMgr
{
public:
  //--------------------------------------------------------------------
  // If this asset has already been loaded, return pointer to shared
  //	instance and increment reference count. Otherwise, load the
  //	resource from the file and create a new instance of the
  //	asset class.
  //--------------------------------------------------------------------
  AssetClass* Load(const LocatorClass& i_Locator)
  {
    std::map<LocatorClass, AssetInfo>::iterator it = sm_Assets.find(i_Locator);
    if (it == sm_Assets.end()) {
      // Load new instance
      AssetClass* pAsset = LoadAsset(i_Locator);
      if (pAsset)
        sm_Assets[i_Locator] = AssetInfo(pAsset);
      return pAsset;
    } else {
      // Increment count of existing asset and return pointer
      ++(it->second.m_Count);
      return it->second.m_pAsset;
    }
  }

  //--------------------------------------------------------------------
  // Release asset. Decrements reference count - when reference
  //	count reaches zero, asset class instance is deleted.
  //	Returns reference counts remaining or -1 if not found.
  //--------------------------------------------------------------------
  int Release(AssetClass* i_pAsset)
  {
    int new_refcount = release_from_map(i_pAsset);
    if (new_refcount >= 0)
      return new_refcount;

    new_refcount = release_from_nonsharing_list(i_pAsset);
    if (new_refcount >= 0)
      return new_refcount;

    return -1; // Use -1 to signify "asset not found"
  }

  //--------------------------------------------------------------------
  // Tell the asset manager to not share the given resource anymore.
  // The next call to Load() for this locator will cause a reload
  // of the original asset.
  // The old asset will remain in management until all the Release()
  // calls are made and its reference count hits 0, but it will not
  // be stored in the map of locator to asset anymore.
  //--------------------------------------------------------------------
  void StopSharing(const LocatorClass& i_Locator)
  {
    std::map<LocatorClass, AssetInfo>::iterator it = sm_Assets.find(i_Locator);
    if (it != sm_Assets.end()) {
      // Add this asset info to the non sharing list
      sm_NonSharingAssets.push_back(it->second);

      // Remove it from the locator map so that the next request
      // will force a new load of the original asset.
      sm_Assets.erase(it);
    }
  }

protected:
  //--------------------------------------------------------------------
  // This function needs to be implemented by base class,
  //	load asset from file and return pointer to new asset class,
  //	allocated on heap.
  //--------------------------------------------------------------------
  virtual AssetClass* LoadAsset(const LocatorClass& i_Locator) = 0;

  //--------------------------------------------------------------------
  // Release asset from locator map
  //--------------------------------------------------------------------
  int release_from_map(AssetClass* i_pAsset)
  {
    std::map<LocatorClass, AssetInfo>::iterator it;
    for (it = sm_Assets.begin(); it != sm_Assets.end(); ++it) {
      if (it->second.m_pAsset == i_pAsset) {
        // Decrement count
        --(it->second.m_Count);

        // If ref count == 0, delete the asset
        int new_refcount = it->second.m_Count;
        if (new_refcount <= 0) {
          delete it->second.m_pAsset;
          sm_Assets.erase(it);
        }

        // Return new reference count
        return new_refcount;
      }
    }

    return -1; // Use -1 to signify "asset not found"
  }

  //--------------------------------------------------------------------
  // Release asset from non-sharing list
  //--------------------------------------------------------------------
  int release_from_nonsharing_list(AssetClass* i_pAsset)
  {
    std::list<AssetInfo>::iterator it;
    for (it = sm_NonSharingAssets.begin(); it != sm_NonSharingAssets.end(); ++it) {
      if (it->m_pAsset == i_pAsset) {
        // Decrement count
        --(it->m_Count);

        // If ref count == 0, delete the asset
        int new_refcount = it->m_Count;
        if (new_refcount <= 0) {
          delete it->m_pAsset;
          sm_NonSharingAssets.erase(it);
        }

        // Return new reference count
        return new_refcount;
      }
    }

    return -1; // Use -1 to signify "asset not found"
  }

  //--------------------------------------------------------------------
  // Internal structure, tracks references per asset
  //--------------------------------------------------------------------
  struct AssetInfo
  {
    AssetInfo()
      : m_pAsset(NULL)
      , m_Count(0)
    {
    }
    AssetInfo(AssetClass* i_pAsset)
      : m_pAsset(i_pAsset)
      , m_Count(1)
    {
    }

    bool operator==(const AssetInfo& i_Info) const { return m_pAsset == i_Info.m_pAsset; }

    AssetClass* m_pAsset;
    int m_Count;
  };

  std::map<LocatorClass, AssetInfo> sm_Assets;
  std::list<AssetInfo> sm_NonSharingAssets;
};
