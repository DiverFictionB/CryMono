using System;
using System.Runtime.CompilerServices;

namespace CryEngine.Native
{
    internal class NativeActorMethods : INativeActorMethods
    {
        #region Externals
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static float _GetPlayerHealth(IntPtr actorPtr);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static void _SetPlayerHealth(IntPtr actorPtr, float newHealth);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static float _GetPlayerMaxHealth(IntPtr actorPtr);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static void _SetPlayerMaxHealth(IntPtr actorPtr, float newMaxHealth);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static ActorInfo _GetActorInfoByChannelId(ushort channelId);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static ActorInfo _GetActorInfoById(uint entId);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern internal static void _RegisterActorClass(string name, bool isNative);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern private static ActorInfo _CreateActor(int channelId, string name, string className, Vec3 pos, Quat rot, Vec3 scale);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static void _RemoveActor(uint id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal static uint _GetClientActorId();
        #endregion


        public void RegisterClass(string className, bool isNative)
        {
			_RegisterActorClass(className, isNative);
        }

        public float GetPlayerHealth(IntPtr actorPtr)
        {
            return _GetPlayerHealth(actorPtr);
        }

        public void SetPlayerHealth(IntPtr actorPtr, float newHealth)
        {
            _SetPlayerHealth(actorPtr, newHealth);
        }

        public float GetPlayerMaxHealth(IntPtr actorPtr)
        {
            return _GetPlayerMaxHealth(actorPtr);
        }

        public void SetPlayerMaxHealth(IntPtr actorPtr, float newMaxHealth)
        {
            _SetPlayerMaxHealth(actorPtr, newMaxHealth);
        }

        public ActorInfo GetActorInfoByChannelId(ushort channelId)
        {
            return _GetActorInfoByChannelId(channelId);
        }

        public ActorInfo GetActorInfoById(uint entId)
        {
            return _GetActorInfoById(entId);
        }

		public ActorInfo CreateActor(int channelId, string name, string className, Vec3 pos, Quat rot, Vec3 scale)
        {
            return _CreateActor(channelId, name, className, pos, rot, scale);
        }

        public void RemoveActor(uint id)
        {
            _RemoveActor(id);
        }

        public uint GetClientActorId()
        {
            return _GetClientActorId();
        }
    }
}