/* Exit Games Photon Chat - C++ Client Lib
 * Copyright (C) 2004-2018 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#pragma once

#include "Common-cpp/inc/Common.h"

namespace ExitGames
{
	namespace Chat
	{
		namespace Internal
		{
			class ChannelFactory;
			class ChannelMessageAdder;
		}

		class Channel : public Common::Base
		{
		public:
			using Common::ToString::toString;

			void clearMessages(void);

			const Common::JString& getName(void) const;
			unsigned int getMessageCount(void) const;
			const Common::JVector<Common::JString>& getSenders(void) const;
			const Common::JVector<Common::Object>& getMessages(void) const;
			bool getIsPrivate(void) const;

			virtual Common::JString& toString(Common::JString& retStr, bool withTypes=false) const;
		private:
			Channel(const Common::JString& name, bool isPrivate);
			void add(const Common::JString& sender, const Common::Object& message);
			void add(const Common::JVector<Common::JString>& senders, const Common::JVector<Common::Object>& messages);

			Common::JString mName;
			Common::JVector<Common::JString> mSenders;
			Common::JVector<Common::Object> mMessages;
			bool mIsPrivate;

			friend class Internal::ChannelFactory;
			friend class Internal::ChannelMessageAdder;
		};
	}
}