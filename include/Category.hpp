#ifndef CATEGORY_HPP
#define CATEGORY_HPP

namespace Category
{
    enum Type
    {
        None = 0,
        Scene = 1<<0,
        Player = 1<<1,
        Enemy = 1<<2,
        EnemyProjectile = 1<<3,
        AlliedProjectile = 1<<4,
        DarkWizzard = 1<<5,
        BossProjectile = 1<<6,
        ParticleSystem = 1<<7,
        SoundEffect = 1<<8,

        Projectile = EnemyProjectile | AlliedProjectile | BossProjectile,
        EnemiesProjectile = EnemyProjectile | BossProjectile,
        Entity = Player | Enemy | DarkWizzard,
    };
}

#endif // CATEGORY_HPP