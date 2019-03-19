/// <summary>
/// シャドウマップの基底クラス。
/// </summary>
#pragma once

namespace tkEngine{
	/// <summary>
	/// シャドウマップの基底クラス。
	/// </summary>
	class IShadowMap : Noncopyable{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		IShadowMap() {}
		/// <summary>
		/// デストラクタ。
		/// </summary>
		virtual ~IShadowMap() {}
	};
}
