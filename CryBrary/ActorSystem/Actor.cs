﻿using System.Runtime.CompilerServices;

namespace CryEngine
{
	/// <summary>
	/// WIP Player class.
	/// </summary>
    public class Actor : Entity
	{
		#region Externals
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern internal static void _RegisterActorClass(string className, bool isAI);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern internal static float _GetPlayerHealth(uint playerId);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern internal static void _SetPlayerHealth(uint playerId, float newHealth);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern internal static float _GetPlayerMaxHealth(uint playerId);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern internal static void _SetPlayerMaxHealth(uint playerId, float newMaxHealth);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern internal static uint _GetEntityIdForChannelId(ushort channelId);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern internal static void _RemoveActor(uint id);
		#endregion

		#region Statics
		public static EntityId GetEntityId(int channelId)
		{
			return new EntityId(_GetEntityIdForChannelId((ushort)channelId));
		}

		public static void Remove(EntityId id)
		{
			_RemoveActor(id);

			Entity.RemoveInternalEntity(id);
		}

		public static void Remove(Actor actor)
		{
			Remove(actor.Id);
		}

		public static void Remove(int channelId)
		{
			Remove(GetEntityId(channelId));
		}
		#endregion

        /// <summary>
        /// Initializes the player.
        /// </summary>
        /// <param name="entityId"></param>
        /// <param name="channelId"></param>
		public void InternalSpawn(EntityId entityId, int channelId)
        {
			ChannelId = channelId;

			// Should be called second last, prior to OnSpawn
			SpawnCommon(entityId);

			OnSpawn();
        }

        public int ChannelId { get; set; }
		public float Health { get { return _GetPlayerHealth(Id); } set { _SetPlayerHealth(Id, value); } }
		public float MaxHealth { get { return _GetPlayerMaxHealth(Id); } set { _SetPlayerMaxHealth(Id, value); } }

        public bool IsDead() { return Health <= 0; }

		internal override bool CanContainEditorProperties { get { return false; } }
    }
}
