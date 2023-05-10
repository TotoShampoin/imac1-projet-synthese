
void drawAABB(PhysicsAABB& box, Image& texture) {
    static Geometry geo = createCube();
    Vec3f center = box.middle();
    Vec3f size = box.size();
    draw3DObject(geo, texture, center, size);
}
void drawSphere(PhysicsSphere& sph) {
    static Geometry geo = createSphere(24);
    Vec3f size = Vec3f(1,1,1) * sph.radius;
    draw3DObject(geo, sph.position, size);
}
void drawLine(Vec3f l1, Vec3f l2) {
    glBegin(GL_LINES);
        glVertex3f(l1.x,l1.y,l1.z);
        glVertex3f(l2.x,l2.y,l2.z);
    glEnd();
}

        // // Test texture
        // gluLookAt(
        //     1, 4, 3,
        //     0, 0, 0,
        //     0, 0, 1
        // );
        // glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        // glPushMatrix();
        // glRotatef(timer * 180 / M_PI, 1, 0, 0);
        // draw3DObject(cube, cube_texture, Vec3f(0, 0, 2.5)); // derrière
        // draw3DObject(cube, transparent, Vec3f(0, 0, 0)); // devant
        // glPopMatrix();

        // // Test collision
        // gluLookAt(
        //     3, 4, 3,
        //     0, 0, 0,
        //     0, 0, 1
        // );
        // glColor4f(1,1,1,1);
        // glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        // glPushMatrix();
        // glRotatef(timer * 180 / M_PI, 1, 0, 0);
        // drawAABB(box, cube_texture);
        // glPopMatrix();
