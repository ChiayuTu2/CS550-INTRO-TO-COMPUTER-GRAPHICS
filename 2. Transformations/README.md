# CS 550 Project Two: Using Transformations to Animate a Carousel Horse!

## Author
- Name: Chiayu Tu
- Email: tuchi@oregonstate.edu

## Project Overview


This project involves creating an animated carousel scene using OpenGL. The project involves programming a carousel horse to travel along a horizontal circle with a specified radius, implementing four types of transformations to simulate realistic motion, and setting up two different camera views for outside and inside perspectives. The animation is controlled by a global time variable to ensure consistent behavior across different systems. The project blends concepts of geometric transformations, camera manipulation, and animation timing to produce an interactive 3D scene.

## Features
- Using OpenGL to draw and animate horses on a circular path
- Transformations include vertical translation, circular revolution, lateral translation, and rocking back and forth.
- Incorporates two viewing perspectives, 'Outside' and 'Inside,' using different gluLookAt() parameters.
- Adding more horses with offset animations.


## How to run
- Download SampleWindows.zip.
- Use VisualStudio 2022.
- If you are on Windows, double-click on the .sln file.
- Download CarouselHorse0.10.550 file and put this file in your folder.
- Draw a wireframe horse, use the following code when you create your horse display list in InitLists():
    ```c
    WireHorseList = glGenLists( 1 );
    glNewList( WireHorseList, GL_COMPILE );
        glPushMatrix( );
            glRotatef(90.f, 0., 1., 0.);
            glTranslatef( 0., -1.1f, 0.f);
            glColor3f( 1.f, 1.f, 0.f); // yellow
            glBegin( GL_LINES );
                for( int i=0; i < HORSEnedges; i++ )
                {
                    struct point p0 = HORSEpoints[ HORSEedges[i].p0 ];
                    struct point p1 = HORSEpoints[ HORSEedges[i].p1 ];
                    glVertex3f( p0.x, p0.y, p0.z );
                    glVertex3f( p1.x, p1.y, p1.z );
                }
            glEnd( );
        glPopMatrix( );
    glEndList( );

- If you want to draw a polygon horse, use the following code when you create your horse display list in InitLists( ):
  ```c
  HorseList = glGenLists( 1 );
	glNewList( HorseList, GL_COMPILE );
		glPushMatrix( );
			glRotatef(90.f, 0., 1., 0.);
			glTranslatef( 0., -1.1f, 0.f);
			glBegin( GL_TRIANGLES );
				for( int i = 0; i < HORSEntris; i++ )
				{
					struct point p0 = HORSEpoints[ HORSEtris[i].p0 ];
					struct point p1 = HORSEpoints[ HORSEtris[i].p1 ];
					struct point p2 = HORSEpoints[ HORSEtris[i].p2 ];

					// fake "lighting" from above:

					float p01[3], p02[3], n[3];
					p01[0] = p1.x - p0.x;
					p01[1] = p1.y - p0.y;
					p01[2] = p1.z - p0.z;
					p02[0] = p2.x - p0.x;
					p02[1] = p2.y - p0.y;
					p02[2] = p2.z - p0.z;
					Cross( p01, p02, n );
					Unit( n, n );
					n[1] = (float)fabs( n[1] );
					// simulating a glColor3f( 1., 1., 0. ) = yellow:
					glColor3f( 1.f*n[1], 1.f*n[1], 0.f*n[1]);

					glVertex3f( p0.x, p0.y, p0.z );
					glVertex3f( p1.x, p1.y, p1.z );
					glVertex3f( p2.x, p2.y, p2.z );
				}
			glEnd( );
		glPopMatrix( );
	glEndList( );

## Project Display
[![Project Display](http://img.youtube.com/vi/6LcDjKYh87E/0.jpg)](http://www.youtube.com/watch?v=6LcDjKYh87E "Project Display")

