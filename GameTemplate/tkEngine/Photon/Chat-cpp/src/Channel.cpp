/* Exit Games Photon Chat - C++ Client Lib
 * Copyright (C) 2004-2018 by Exit Games GmbH. All rights reserved.
 * http://www.photonengine.com
 * mailto:developer@photonengine.com
 */

#include "Chat-cpp/inc/Channel.h"

/** @file Channel.h */

namespace ExitGames
{
	namespace Chat
	{
		using namespace Common;
		using namespace Common::MemoryManagement;

		/**
		   @class Channel
		   Represents channel or private chat (channel with 2 users)*/

		Channel::Channel(const JString& name, bool isPrivate)
			: mName(name)
			, mIsPrivate(isPrivate)
		{
		}

		const JString& Channel::getName(void) const
		{
			return mName;
		}

		unsigned int Channel::getMessageCount(void) const
		{
			return mMessages.getSize();
		}

		const JVector<JString>& Channel::getSenders(void) const
		{
			return mSenders;
		}

		const JVector<Object>& Channel::getMessages(void) const
		{
			return mMessages;
		}

		void Channel::add(const JString& sender, const Object& message)
		{
			mSenders.addElement(sender);
			mMessages.addElement(message);
		}

		void Channel::add(const JVector<JString>& senders, const JVector<Object>& messages)
		{
			for(unsigned int i=0; i<senders.getSize(); ++i)
				mSenders.addElement(senders[i]);
			for(unsigned int i=0; i<senders.getSize(); ++i)
				mMessages.addElement(messages[i]);
		}

		void Channel::clearMessages(void)
		{
			mSenders.removeAllElements();
			mMessages.removeAllElements();
		}

		bool Channel::getIsPrivate(void) const 
		{
			return mIsPrivate;
		}

		JString& Channel::toString(JString& retStr, bool withTypes) const
		{
			return retStr += L"ChatChannel " + mName;
		}
	}
}