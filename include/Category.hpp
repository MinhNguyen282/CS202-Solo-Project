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

        Projectile = EnemyProjectile | AlliedProjectile,
    };
}

#endif // CATEGORY_HPP