/* 
>> RakNet Manager plugin by urShadow
>> Version 1.0.0
>> 2016 
 */
 
#if defined _raknetmanager_included
  #endinput
#endif
#define _raknetmanager_included

/*
>> BitStream's types
*/
enum
{
	BS_INT8,
	BS_INT16,
	BS_INT32,
	BS_UINT8,
	BS_UINT16,
	BS_UINT32,	
	BS_FLOAT,
	BS_BOOL,	
	BS_STRING,
	
	// compressed values
	BS_CINT8, 
	BS_CINT16,
	BS_CINT32,
	BS_CUINT8,
	BS_CUINT16,
	BS_CUINT32,	
	BS_CFLOAT,
	BS_CBOOL,	
}

/*
>> Send data to client. Fresh RPC/Packet ids you can get here:
>> https://github.com/P3ti/RakSAMP/blob/master/raknet/SAMP/SAMPRPC.cpp
>> https://github.com/P3ti/RakSAMP/blob/master/raknet/PacketEnumerations.h
>> playerid == -1 => send to all players
*/
native SendRPC(playerid, rpcid, BitStream:bs);  
native SendPacket(playerid, BitStream:bs);

native BitStream:BS_New();
native BS_Delete(&BitStream:bs);

native BS_Reset(BitStream:bs);
native BS_ResetReadPointer(BitStream:bs);
native BS_ResetWritePointer(BitStream:bs);
native BS_IgnoreBits(BitStream:bs, bitcount);

native BS_SetWriteOffset(BitStream:bs, offset);
native BS_GetWriteOffset(BitStream:bs, &offset);
native BS_SetReadOffset(BitStream:bs, offset);
native BS_GetReadOffset(BitStream:bs, &offset);

native BS_GetNumberOfBitsUsed(BitStream:bs, &number);
native BS_GetNumberOfBytesUsed(BitStream:bs, &number);
native BS_GetNumberOfUnreadBits(BitStream:bs, &number);

native BS_WriteValue(BitStream:bs, {Float,_}:...);
native BS_ReadValue(BitStream:bs, {Float,_}:...);

/*
>> Calls on incoming data from client
*/
forward OnPlayerReceivedPacket(playerid, packetid, BitStream:bs);
forward OnPlayerReceivedRPC(playerid, rpcid, BitStream:bs);

/*
>> Calls on outcoming data to client
*/
forward OnServerSendPacket(playerid, packetid, BitStream:bs);
forward OnServerSendRPC(playerid, rpcid, BitStream:bs);

/*
>> Examples (0.3.7-R2)

1. Sending RPC

stock SendChatMessage(senderid, msg[])
{
  new BitStream:bs = BS_New(), RPC_Chat = 101;

  BS_WriteValue(bs, BS_INT16, senderid, 
                    BS_INT8, strlen(msg), 
                    BS_STRING, msg);

  SendRPC(senderid, RPC_Chat, bs);

  BS_Delete(bs);
}

2. Sending packet

stock FakeLostConnectionForPlayer(playerid)
{
  new BitStream:bs = BS_New(), ID_CONNECTION_LOST = 33;

  BS_WriteValue(bs, BS_INT8, ID_CONNECTION_LOST);

  SendPacket(playerid, bs);

  BS_Delete(bs);
}

3. Intercept incoming onfoot sync of player 

new ID_PLAYER_SYNC = 207;
public OnPlayerReceivedPacket(playerid, packetid, BitStream:bs)
{
	if(packetid == ID_PLAYER_SYNC)
	{
	new lrkeys, udkeys, sampkeys,
	Float:pos[3], Float:quaternion[4],
	health, armor, weaponid, specialaction,
	Float:speed[3], Float:surfingoffsets[3],
	surfingvehid, animationid, animflags;
	 
    BS_IgnoreBits(bs, 8); // packet id 1 byte
	BS_ReadValue(bs, BS_UINT16, lrkeys,
					 BS_UINT16, udkeys,
				     BS_UINT16, sampkeys,
					 BS_FLOAT, pos[0],
					 BS_FLOAT, pos[1],
					 BS_FLOAT, pos[2],
					 BS_FLOAT, quaternion[0],
					 BS_FLOAT, quaternion[1],
                     BS_FLOAT, quaternion[2],
                     BS_FLOAT, quaternion[3],
	                 BS_UINT8, health,
                     BS_UINT8, armor,
	                 BS_UINT8, weaponid,
                     BS_UINT8, specialaction,
                     BS_FLOAT, speed[0],
					 BS_FLOAT, speed[1],
					 BS_FLOAT, speed[2],
					 BS_FLOAT, surfingoffsets[0],
					 BS_FLOAT, surfingoffsets[1],
					 BS_FLOAT, surfingoffsets[2],
					 BS_UINT16, surfingvehid,
					 BS_INT16, animationid,
					 BS_INT16, animflags);
					  
	printf("lrkeys %d udkeys %d sampkeys %d\n" \
	       "pos %.2f %.2f %.2f\n" \
		   "quaternion %.2f %.2f %.2f %.2f\n" \
		   "health %d\n" \
		   "armor %d\n" \
		   "weaponid %d\n" \
		   "specialaction %d\n" \
		   "speed %.2f %.2f %.2f\n" \
		   "surfingoffsets %.2f %.2f %.2f\n" \
		   "surfingvehid %d\n" \
		   "anim %d flags %d",
		   lrkeys, udkeys, sampkeys,
	       pos[0], pos[1], pos[2],
	       quaternion[0], quaternion[1], quaternion[2], quaternion[3],
	       health, armor, weaponid, specialaction,
	       speed[0], speed[1], speed[2],
		   surfingoffsets[0], surfingoffsets[1], surfingoffsets[2],
	       surfingvehid, animationid, animflags
		   );
    }

	return 1;
}

4. Intercept incoming chat message rpc

new RPC_Chat = 101;
public OnPlayerReceivedRPC(playerid, rpcid, BitStream:bs)
{
	if(rpcid == RPC_Chat) 
	{
	new message[256], len;
	
	BS_ReadValue(bs, BS_UINT8, len,
					 BS_STRING, message, len);

	printf("chat message from player %d: %s", playerid, message);
	}
	
	return 1;
} 

*/