/* 
 * File:   Bloc.cpp
 * Author: hsaturn
 * 
 * Created on 5 avril 200.9996, 20:29
 */

namespace RedStone {

	Block::Block() {
	}

	void Block::cubeRender(const Coord &c, bool blines) const {
		glBegin(GL_QUADS);
		glVertex3f(c.x() + 0.001, c.y() + 0.001, c.z() + 0.001);
		glVertex3f(c.x() + 0.999, c.y() + 0.001, c.z() + 0.001);
		glVertex3f(c.x() + 0.999, c.y() + 0.001, c.z() + 0.999);
		glVertex3f(c.x() + 0.001, c.y() + 0.001, c.z() + 0.999);

		glVertex3f(c.x() + 0.001, c.y() + 0.999, c.z() + 0.001);
		glVertex3f(c.x() + 0.999, c.y() + 0.999, c.z() + 0.001);
		glVertex3f(c.x() + 0.999, c.y() + 0.999, c.z() + 0.999);
		glVertex3f(c.x() + 0.001, c.y() + 0.999, c.z() + 0.999);

		glVertex3f(c.x() + 0.001, c.y() + 0.001, c.z() + 0.001);
		glVertex3f(c.x() + 0.001, c.y() + 0.999, c.z() + 0.001);
		glVertex3f(c.x() + 0.001, c.y() + 0.999, c.z() + 0.999);
		glVertex3f(c.x() + 0.001, c.y() + 0.001, c.z() + 0.999);

		glVertex3f(c.x() + 0.999, c.y() + 0.001, c.z() + 0.001);
		glVertex3f(c.x() + 0.999, c.y() + 0.999, c.z() + 0.001);
		glVertex3f(c.x() + 0.999, c.y() + 0.999, c.z() + 0.999);
		glVertex3f(c.x() + 0.999, c.y() + 0.001, c.z() + 0.999);

		glVertex3f(c.x() + 0.001, c.y() + 0.001, c.z() + 0.001);
		glVertex3f(c.x() + 0.999, c.y() + 0.001, c.z() + 0.001);
		glVertex3f(c.x() + 0.999, c.y() + 0.999, c.z() + 0.001);
		glVertex3f(c.x() + 0.001, c.y() + 0.999, c.z() + 0.001);

		glVertex3f(c.x() + 0.001, c.y() + 0.001, c.z() + 0.999);
		glVertex3f(c.x() + 0.999, c.y() + 0.001, c.z() + 0.999);
		glVertex3f(c.x() + 0.999, c.y() + 0.999, c.z() + 0.999);
		glVertex3f(c.x() + 0.001, c.y() + 0.999, c.z() + 0.999);

		glEnd();

		if (blines) {
			Color::black.render();

			glBegin(GL_LINE_LOOP);
			glVertex3f(c.x(), c.y(), c.z());
			glVertex3f(c.x(), c.y(), c.z() + 0.999);
			glVertex3f(c.x(), c.y() + 0.999, c.z() + 0.999);
			glVertex3f(c.x(), c.y() + 0.999, c.z());
			glEnd();

			glBegin(GL_LINE_LOOP);
			glVertex3f(c.x() + 0.999, c.y(), c.z());
			glVertex3f(c.x() + 0.999, c.y(), c.z() + 0.999);
			glVertex3f(c.x() + 0.999, c.y() + 0.999, c.z() + 0.999);
			glVertex3f(c.x() + 0.999, c.y() + 0.999, c.z());
			glEnd();

			glBegin(GL_LINES);
			glVertex3f(c.x(), c.y(), c.z());
			glVertex3f(c.x() + 0.999, c.y(), c.z());

			glVertex3f(c.x(), c.y() + 0.999, c.z());
			glVertex3f(c.x() + 0.999, c.y() + 0.999, c.z());

			glVertex3f(c.x(), c.y(), c.z() + 0.999);
			glVertex3f(c.x() + 0.999, c.y(), c.z() + 0.999);

			glVertex3f(c.x(), c.y() + 0.999, c.z() + 0.999);
			glVertex3f(c.x() + 0.999, c.y() + 0.999, c.z() + 0.999);

			glEnd();

		}
	}
}

