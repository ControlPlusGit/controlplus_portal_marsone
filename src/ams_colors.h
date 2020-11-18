/*
 *****************************************************************************
 * Copyright by ams AG                                                       *
 * All rights are reserved.                                                  *
 *                                                                           *
 * IMPORTANT - PLEASE READ CAREFULLY BEFORE COPYING, INSTALLING OR USING     *
 * THE SOFTWARE.                                                             *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         *
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS         *
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  *
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     *
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT          *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     *
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      *
 *****************************************************************************
 */

/*! \file ams_colors.h
 *
 *  \author F. Lobmaier
 *
 *  \brief Constants shared between GUI and firmware concerning the ams
 *         corporate colors.
 */

#ifndef AMS_COLORS_H
#define AMS_COLORS_H

// AMS color definitions
// defined as strings to be easily used with Qt stylesheet definitions
// (e.g. for USB indicator in statusbar)
// format for firmware usage to be defined
#define AMSCOLOR_GRAY_95	"#2c292d"
#define AMSCOLOR_GRAY 		"#46555f"
#define AMSCOLOR_GRAY_70 	"#7d888f"
#define AMSCOLOR_GRAY_45	"#acb2b7"

#define AMSCOLOR_YELLOW		"#f7e600"

#define AMSCOLOR_BLUE		"#0075b0"
#define AMSCOLOR_BLUE_70	"#4c9ec8"
#define AMSCOLOR_BLUE_45	"#8cc1db"

#define AMSCOLOR_GREEN		"#009f69"
#define AMSCOLOR_LIGHTGREEN "#78f078"

#define AMSCOLOR_RED		"#df5353"

#define AMSCOLOR_VIOLA		"#8b2dbd"

#define AMSCOLOR_BLACKBERRY "#654447"
#define AMSCOLOR_SKIN		"#ffb690"
#define AMSCOLOR_WOOD		"#8c6239"

// AMS color macros
#define AMSCOLOR_CSS_COLOR(value) QString("color: ") + QString(value) + QString(";")
#define AMSCOLOR_CSS_BGCOLOR(value) QString("background-color: ") + QString(value) + QString(";")


#endif //AMS_COLORS_H
