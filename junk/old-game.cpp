/* on_mouse_move */
// player.racket.position = Vec3f(
//     clamp(xpos, -(LEVEL_WIDTH - 0.3), LEVEL_WIDTH - 0.3), 
//     clamp(ypos, -(LEVEL_HEIGHT - 0.3), LEVEL_HEIGHT - 0.3), 
//     player.racket.position.z
// );
// player.racket.hitbox.boundA = Vec3f(-1 * player.racket.scale.x,
//                                 -1 * player.racket.scale.y,
//                                 0.0) + player.racket.position;
// player.racket.hitbox.boundB = Vec3f(1 * player.racket.scale.x,
//                                 1 * player.racket.scale.y,
//                                 -0.1) + player.racket.position;
// if (player.racket.hasBall) {
//     player.ball.position = player.racket.position + Vec3f(0, 0, 1);
// }

/* physics */
// player.ball.update(delta_time);

// player.ball.collide(level.walls);
// player.ball.collide(level.obstacles);
// player.ball.collide(player.racket.hitbox);

// if (player.racket.isMovingForward) {
//     player.racket.position.z += 0.025;
//     player.racket.hitbox.boundA = Vec3f(-1 * player.racket.scale.x,
//                                         -1 * player.racket.scale.y,
//                                         0.0) + player.racket.position;
//     player.racket.hitbox.boundB = Vec3f(1 * player.racket.scale.x,
//                                         1 * player.racket.scale.y,
//                                         -0.1) + player.racket.position;
// }

// Level temporaryLevel() {
//     Level level (20);
//     level.obstacles.push_back(Obstacle {
//         Vec3f(0, -DEFAULT_LEVEL_HEIGHT, 5),
//         Vec3f(DEFAULT_LEVEL_WIDTH, DEFAULT_LEVEL_HEIGHT, 5.5)
//     });
//     level.obstacles.push_back(Obstacle {
//         Vec3f(-DEFAULT_LEVEL_WIDTH, -DEFAULT_LEVEL_HEIGHT, 10),
//         Vec3f(0, DEFAULT_LEVEL_HEIGHT, 10.5)
//     });
//     return level;
// }
