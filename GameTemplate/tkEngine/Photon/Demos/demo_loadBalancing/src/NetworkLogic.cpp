#include "NetworkLogic.h"

#ifdef _EG_EMSCRIPTEN_PLATFORM
#	include <emscripten.h>
#endif

static const EG_CHAR* appID = L"<no-app-id>"; // set your app id here
static const EG_CHAR* appVersion = L"1.0";

static const bool autoLobbyStats = true;
static const int sendInterval = 1000;
static const bool useBestRegion = false; // attention: in RegionSelectionMode::BEST different client instances (sometimes even from the same location) might get different ping results and hence might connect to different regions - in that case they don't see each other or each others rooms!
#if defined EG_PLATFORM_SUPPORTS_CPP11 && defined EG_PLATFORM_SUPPORTS_MULTITHREADING
	// pinging takes a moment, so for a real game it makes sense to retrive the best region with getRegionWithBestPing(), store it in a file, use RegionSelectionMode::SELECT and pass that region to selectRegion() and to only use RegionSelectionMode::BEST, if no best region has been determined yet or if the player explicitly requests to repeat pinging
	static const nByte regionSelectionMode = useBestRegion?ExitGames::LoadBalancing::RegionSelectionMode::BEST:ExitGames::LoadBalancing::RegionSelectionMode::SELECT;
#else
	static const nByte regionSelectionMode = ExitGames::LoadBalancing::RegionSelectionMode::SELECT;
#endif

static const EG_CHAR* PLAYER_NAME =
#if defined _EG_WINDOWS_PLATFORM
	L"Windows";
#elif defined _EG_IPHONE_PLATFORM
	L"iOS";
#elif defined _EG_IMAC_PLATFORM
	L"OS X";
#elif defined _EG_MARMALADE_PLATFORM
	L"Marmalade";
#elif defined _EG_ANDROID_PLATFORM
	L"Android";
#elif defined _EG_PS3_PLATFORM
	L"PS3";
#elif defined _EG_LINUX_PLATFORM
	L"Linux";
#elif defined _EG_PS4_PLATFORM
	L"PS4";
#elif defined _EG_WINDOWSSTORE_PLATFORM
	L"Windows Store";
#elif defined _EG_EMSCRIPTEN_PLATFORM
	L"Emscripten";
#elif defined _EG_PSVITA_PLATFORM
	L"PS Vita";
#elif defined _EG_XB1_PLATFORM
	L"XB1";
#elif defined _EG_SWITCH_PLATFORM
	L"Switch";
#else
	L"unknown platform";
#endif

ExitGames::Common::JString& NetworkLogicListener::toString(ExitGames::Common::JString& retStr, bool /*withTypes*/) const
{
	return retStr;
}

State StateAccessor::getState(void) const
{
	return mState;
}

void StateAccessor::setState(State newState)
{
	mState = newState;
	for(unsigned int i=0; i<mStateUpdateListeners.getSize(); i++)
		mStateUpdateListeners[i]->stateUpdate(newState);
}

void StateAccessor::registerForStateUpdates(NetworkLogicListener* listener)
{
	mStateUpdateListeners.addElement(listener);
}



Input NetworkLogic::getLastInput(void) const
{
	return mLastInput;
}

void NetworkLogic::setLastInput(Input newInput)
{
	mLastInput = newInput;
}

State NetworkLogic::getState(void) const
{
	return mStateAccessor.getState();
}

#ifdef _EG_XB1_PLATFORM
void NetworkLogic::setXSTSToken(const ExitGames::Common::JVector<nByte>& XSTSToken)
{
	if(mXSTSToken == XSTSToken)
		return;
	mXSTSToken = XSTSToken;
	mReauthenticateRequired = true;
}
#endif

// functions
NetworkLogic::NetworkLogic(OutputListener* listener)
#ifdef _EG_MS_COMPILER
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif
	: mLoadBalancingClient(*this, appID, appVersion, ExitGames::Photon::ConnectionProtocol::DEFAULT, autoLobbyStats, regionSelectionMode)
	, mLastPlayerNr(0)
	, mLastInput(INPUT_NON)
	, mpOutputListener(listener)
#if defined _EG_EMSCRIPTEN_PLATFORM || defined _EG_PSVITA_PLATFORM || defined _EG_XB1_PLATFORM || defined _EG_SWITCH_PLATFORM
	, mAutoJoinRoom(true)
#else
	, mAutoJoinRoom(false)
#endif
	, mLastSendTime(GETTIMEMS())
#ifdef _EG_XB1_PLATFORM
	, mReauthenticateRequired(false)
#endif
#ifdef _EG_MS_COMPILER
#	pragma warning(pop)
#endif
{
	mStateAccessor.setState(STATE_INITIALIZED);
	mLoadBalancingClient.setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // that instance of LoadBalancingClient and its implementation details
	mLogger.setListener(*this);
	mLogger.setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // this class
	ExitGames::Common::Base::setListener(this);
	ExitGames::Common::Base::setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // all classes that inherit from Base
}

void NetworkLogic::registerForStateUpdates(NetworkLogicListener* listener)
{
	mStateAccessor.registerForStateUpdates(listener);
}

void NetworkLogic::connect(void)
{
	mpOutputListener->writeLine(ExitGames::Common::JString(L"connecting to Photon"));
	static const ExitGames::Common::JString USER_ID = ExitGames::Common::JString()+GETTIMEMS();
	ExitGames::LoadBalancing::AuthenticationValues authValues;
	authValues.setUserID(USER_ID);
#ifdef _EG_XB1_PLATFORM
	authValues.setType(ExitGames::LoadBalancing::CustomAuthenticationType::XBOX).setData(mXSTSToken);
	mReauthenticateRequired = false;
#endif
	mLoadBalancingClient.connect(authValues, PLAYER_NAME);
	mStateAccessor.setState(STATE_CONNECTING);
}

void NetworkLogic::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

void NetworkLogic::opCreateRoom(nByte directMode)
{
	ExitGames::Common::JString name;
	name += GETTIMEMS();
	mLoadBalancingClient.opCreateRoom(name, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(4).setPlayerTtl(INT_MAX/2).setEmptyRoomTtl(10000).setDirectMode(directMode));
	mStateAccessor.setState(STATE_JOINING);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"creating room ") + name + L"...");
}

void NetworkLogic::opJoinRandomRoom(void)
{
	mLoadBalancingClient.opJoinRandomRoom();
}

void NetworkLogic::opJoinOrCreateRoom(void)
{
	ExitGames::Common::JString name(L"DemoLoadBalancing");
	mLoadBalancingClient.opJoinOrCreateRoom(name);
	mStateAccessor.setState(STATE_JOINING);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"joining or creating room ") + name + L"...");
}

void NetworkLogic::run(void)
{
	State state = mStateAccessor.getState();
	ExitGames::Common::EGTime t = GETTIMEMS();
	if(mLastInput == INPUT_EXIT && state != STATE_DISCONNECTING && state != STATE_DISCONNECTED)
	{
		disconnect();
		mStateAccessor.setState(STATE_DISCONNECTING);
		mpOutputListener->writeLine(L"terminating application");
	}
	else
	{
		switch(state)
		{
			case STATE_INITIALIZED:
				connect();
				break;
			case STATE_CONNECTING:
				break; // wait for callback
			case STATE_CONNECTED:
				switch(mLastInput)
				{
					case INPUT_1:
						mpOutputListener->writeLine(L"\n============= Create Game");
						opCreateRoom(ExitGames::LoadBalancing::DirectMode::NONE);
						break;
					case INPUT_2:
						mpOutputListener->writeLine(L"\n============= Join Random Game");
						// remove false to enable rejoin
						if((false) && mLastJoinedRoom.length())
						{
							mpOutputListener->writeLine(ExitGames::Common::JString(L"rejoining ") + mLastJoinedRoom + "...");
							mLoadBalancingClient.opJoinRoom(mLastJoinedRoom, true);
						}
						else
						{
							mpOutputListener->writeLine(ExitGames::Common::JString(L"joining random room..."));
							opJoinRandomRoom();
						}
						mStateAccessor.setState(STATE_JOINING);
						break;
#ifndef _EG_EMSCRIPTEN_PLATFORM
					case INPUT_3:
						mpOutputListener->writeLine(L"\n============= Create Game Direct All To All");
						opCreateRoom(ExitGames::LoadBalancing::DirectMode::ALL_TO_ALL);
						break;
					case INPUT_4:
						mpOutputListener->writeLine(L"\n============= Create Game Direct Master To All");
						opCreateRoom(ExitGames::LoadBalancing::DirectMode::MASTER_TO_ALL);
						break;
#endif
					default: // no or illegal input -> stay waiting for legal input
						break;
				}
				break;
			case STATE_JOINING:
				break; // wait for callback
			case STATE_JOINED:				
				if(mLastSendTime+sendInterval < t)
				{
					mLastSendTime = t;
					sendEvent();
				}
				switch(mLastInput)
				{
				case INPUT_1: // leave Game
					mLoadBalancingClient.opLeaveRoom();
					mpOutputListener->writeLine(L"");
					mpOutputListener->writeLine(L"leaving room");
					mStateAccessor.setState(STATE_LEAVING);
					break;
				case INPUT_2: // leave Game
					mLoadBalancingClient.opLeaveRoom(true);
					mpOutputListener->writeLine(L"");
					mpOutputListener->writeLine(L"leaving room (will come back)");
					mStateAccessor.setState(STATE_LEAVING);
					break;
				default: // no or illegal input -> stay waiting for legal input
					break;
				}
				break;
			case STATE_LEAVING:
				break; // wait for callback
			case STATE_LEFT:
#ifdef _EG_XB1_PLATFORM
				if(mReauthenticateRequired)
				{
					EGLOG(ExitGames::Common::DebugLevel::INFO, L"reauthenticating");
					mStateAccessor.setState(STATE_DISCONNECTING);
					disconnect();
				}
				else
#endif
					mStateAccessor.setState(STATE_CONNECTED);
				break;
			case STATE_DISCONNECTING:
				break; // wait for callback
			default:
				break;
		}
	}
	mLastInput = INPUT_NON;
	mLoadBalancingClient.service();
}

void NetworkLogic::sendEvent(void)
{
	static int64 count = 0;
	if(mLoadBalancingClient.getCurrentlyJoinedRoom().getDirectMode() == ExitGames::LoadBalancing::DirectMode::NONE)
	{
		mLoadBalancingClient.opRaiseEvent(false, count, 0);
		mpOutputListener->write(ExitGames::Common::JString(L"S") + count + L" ");
	}
	else
		sendDirect(count);
	++count;
#if defined _EG_EMSCRIPTEN_PLATFORM
	mpOutputListener->writeLine(L"");
#endif
}

void NetworkLogic::sendDirect(int64 count)
{
	int localNr = mLoadBalancingClient.getLocalPlayer().getNumber();
	ExitGames::Common::JString str = ExitGames::Common::JString(L"") + count + L"/p" + localNr;

	const int sendMode = 0;
	const bool fallbackRelay = true;
	switch(sendMode)
	{
	case 0: // send to all with one call
		{				
			int sent = mLoadBalancingClient.sendDirect(str, ExitGames::Common::JVector<int>(), fallbackRelay);
			mpOutputListener->write(L"Sd" + str + L"->ALL(" + sent + ") ");
		}
		break;
	case 1: // send to a few with one call
		{
			ExitGames::Common::JVector<int> targets;
			for(int i=0; i<mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayerCount() && targets.getSize()<10; ++i)
			{
				int nr = mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayers()[i]->getNumber();
				if(nr%2)
					targets.addElement(nr);
			}
			int sent = mLoadBalancingClient.sendDirect(str, targets, fallbackRelay);
			mpOutputListener->write(L"Sd" + str + L"->p" + targets.toString() + "(" + sent + ") ");
		}
		break;
	case 2: // send with call per player			
		{
			for(int i=0; i<mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayerCount(); ++i) 
			{
				int nr = mLoadBalancingClient.getCurrentlyJoinedRoom().getPlayers()[i]->getNumber();
				if(nr != localNr)
				{
					if(mLoadBalancingClient.sendDirect(str, nr, fallbackRelay))
						mpOutputListener->write(L"Sd" + str + L"->p" + nr + L" ");
					else 
						mpOutputListener->write(L"Xd" + str + L"->p" + nr + L" ");
				}
			}
		}
		break;
	default:
		break;
	}
}

// protocol implementations

void NetworkLogic::debugReturn(int debugLevel, const ExitGames::Common::JString& string)
{
	mpOutputListener->debugReturn(debugLevel, string);
}

void NetworkLogic::connectionErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"received connection error ") + errorCode);
	mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkLogic::clientErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"received error ") + errorCode + L" from client");
}

void NetworkLogic::warningReturn(int warningCode)
{
	EGLOG(ExitGames::Common::DebugLevel::WARNINGS, L"code: %d", warningCode);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"received warning ") + warningCode + L" from client");
}

void NetworkLogic::serverErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mpOutputListener->writeLine(ExitGames::Common::JString(L"received error ") + errorCode + " from server");
}

void NetworkLogic::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& /*playernrs*/, const ExitGames::LoadBalancing::Player& player)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"%ls joined the game", player.getName().cstr());
	mpOutputListener->writeLine(L"");
	mpOutputListener->writeLine(ExitGames::Common::JString(L"player ") + playerNr + L" " + player.getName() + L" has joined the game");
}

void NetworkLogic::leaveRoomEventAction(int playerNr, bool isInactive)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"");
	mpOutputListener->writeLine(ExitGames::Common::JString(L"player ") + playerNr + L" has left the game");
}

void NetworkLogic::customEventAction(int playerNr, nByte /*eventCode*/, const ExitGames::Common::Object& eventContent)
{
	// you do not receive your own events, unless you specify yourself as one of the receivers explicitly, so you must start 2 clients, to receive the events, which you have sent, as sendEvent() uses the default receivers of opRaiseEvent() (all players in same room like the sender, except the sender itself)
	EGLOG(ExitGames::Common::DebugLevel::ALL, L"");
	mpOutputListener->write(ExitGames::Common::JString(L"R") + ExitGames::Common::ValueObject<long long>(eventContent).getDataCopy() + "<-p" + playerNr + L" ");
#if defined _EG_EMSCRIPTEN_PLATFORM
	mpOutputListener->writeLine(L"");
#endif
}

void NetworkLogic::onDirectMessage(const ExitGames::Common::Object& msg, int remoteID, bool relay)
{
	mpOutputListener->write(ExitGames::Common::JString(L"Rd") + (relay?L"(R)":L"") + msg.toString() + "<-p" + remoteID + L" ");
}

void NetworkLogic::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster)
{
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mStateAccessor.setState(STATE_DISCONNECTING);
		return;
	}
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"connected to cluster " + cluster + L" of region " + region);
	mpOutputListener->writeLine(L"connected to cluster " + cluster + L" of region " + region);
	mStateAccessor.setState(STATE_CONNECTED);
	if(mAutoJoinRoom)
		opJoinOrCreateRoom();
}

void NetworkLogic::disconnectReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"disconnected");
#ifdef _EG_XB1_PLATFORM
	mStateAccessor.setState(mReauthenticateRequired?STATE_INITIALIZED:STATE_DISCONNECTED);
#else
	mStateAccessor.setState(STATE_DISCONNECTED);
#endif
}

void NetworkLogic::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opCreateRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();
	mLastPlayerNr = localPlayerNr;

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been created");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinOrCreateRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();
	mLastPlayerNr = localPlayerNr;

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been entered");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		mLastJoinedRoom = L"";
		mLastPlayerNr = 0;
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been successfully joined");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opJoinRandomRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}

	mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();
	mLastPlayerNr = localPlayerNr;

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mpOutputListener->writeLine(L"... room " + mLoadBalancingClient.getCurrentlyJoinedRoom().getName() + " has been successfully joined");
	mpOutputListener->writeLine(L"regularly sending dummy events now");
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mpOutputListener->writeLine(L"opLeaveRoom() failed: " + errorString);
		mStateAccessor.setState(STATE_DISCONNECTING);
		return;
	}
	mStateAccessor.setState(STATE_LEFT);
	mpOutputListener->writeLine(L"room has been successfully left");
}

void NetworkLogic::joinLobbyReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"joined lobby");
}

void NetworkLogic::leaveLobbyReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mpOutputListener->writeLine(L"left lobby");
}

void NetworkLogic::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"%ls", lobbyStats.toString().cstr());
	mpOutputListener->writeLine(L"LobbyStats: " + lobbyStats.toString());
}

void NetworkLogic::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"%ls", lobbyStats.toString().cstr());
	mpOutputListener->writeLine(L"LobbyStats: " + lobbyStats.toString());
}

void NetworkLogic::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"%ls / %ls", availableRegions.toString().cstr(), availableRegionServers.toString().cstr());
	mpOutputListener->writeLine(L"onAvailableRegions: " + availableRegions.toString() + L" / " + availableRegionServers.toString());
	// select first region from list
	ExitGames::Common::JString selectedRegion;
	mpOutputListener->writeLine(L"selecting region: " + (selectedRegion=availableRegions[0]));
	mLoadBalancingClient.selectRegion(selectedRegion);
}