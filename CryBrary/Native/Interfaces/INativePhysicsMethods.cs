﻿using System;
using System.Runtime.CompilerServices;

namespace CryEngine.Native
{
    interface INativePhysicsMethods
    {
        IntPtr GetPhysicalEntity(IntPtr entityPointer);

        int RayWorldIntersection(Vec3 origin, Vec3 dir, EntityQueryFlags objFlags, RayWorldIntersectionFlags flags, out RaycastHit rayHit, int maxHits, object[] skipEnts);

        void Physicalize(IntPtr entPtr, PhysicalizationParams physicalizationParams);
        void Sleep(IntPtr entPtr, bool sleep);

        void AddImpulse(IntPtr entPtr, pe_action_impulse actionImpulse);

        Vec3 GetVelocity(IntPtr entPtr);
        void SetVelocity(IntPtr entPtr, Vec3 velocity);

        object[] SimulateExplosion(pe_explosion explosion);

        pe_status_living GetLivingEntityStatus(IntPtr entPtr);

        pe_action_impulse GetImpulseStruct();
        pe_player_dimensions GetPlayerDimensionsStruct();
        pe_player_dynamics GetPlayerDynamicsStruct();
    }
}
