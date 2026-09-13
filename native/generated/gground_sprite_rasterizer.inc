// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
// Generated native storage adaptation; original rasterization loops retained.
// Source SHA256: 9c909d9e6354d9ce3ac9a4d1a07f7bf4344fc417a8900eae0d1094176704ced4
void draw_sprites(std::vector<uint16_t> &sprite_ram, bitmap_ind16 &bitmap, const rectangle &cliprect, bitmap_ind8 &priority_bitmap, const int *spri)
{
	uint16_t curspr = 0;
	int countspr = 0;
	uint8_t pmt[4];
	uint16_t *sprd[0x2000], *clip[0x2000];
	uint16_t *cclip = nullptr;

	for(int i=0; i<4; i++)
		pmt[i] = 0xff << (1+spri[3-i]);

	for(int seen = 0; seen < 0x2000; seen++) {
		uint16_t *source;
		uint16_t type;

		source = sprite_ram.data() + (curspr << 3);

		if(curspr == 0 && source[0] == 0)
			break;

		curspr = source[0];
		type = curspr & 0xc000;
		curspr &= 0x1fff;

		if(type == 0xc000)
			break;

		if(type == 0x8000)
			continue;

		if(type == 0x4000) {
			cclip = source;
			continue;
		}

		sprd[countspr] = source;
		clip[countspr] = cclip;

		countspr++;
		if(!curspr)
			break;
	}

	
	for(countspr--; countspr >= 0; countspr--) {
		int x, y, sx, sy;
		uint16_t colors[16];
		int flipx, flipy;
		//      int dump;

		const uint16_t *const source = sprd[countspr];
		cclip = clip[countspr];

		int min_x, min_y, max_x, max_y, clip_first, clip_last;
		if(cclip) {
			// Crackdown uses this on pre-title screen intro
			// for masking both avatars and the Sega logo itself.
			const bool clip_reverse_y = (cclip[1] & 0x2000) >> 13;
			min_x = std::max<int>(cliprect.min_x, (cclip[3] & 511) - 8);
			max_x = std::min<int>(cliprect.max_x, (cclip[5] & 511) - 8);
			if(clip_reverse_y) {
				min_y = std::max<int>(cliprect.min_y, 0);
				max_y = std::min<int>(cliprect.max_y, 383);
				clip_first = (cclip[2] & 511) - 1;
				clip_last = (cclip[4] & 511) + 1;
			} else {
				min_y = std::max<int>(cliprect.min_y, cclip[2] & 511);
				max_y = std::min<int>(cliprect.max_y, cclip[4] & 511);
				clip_first = max_y;
				clip_last = min_y;
			}
		} else {
			min_x = std::max<int>(cliprect.min_x, 0);
			max_x = std::min<int>(cliprect.max_x, 495);
			min_y = std::max<int>(cliprect.min_y, 0);
			max_y = std::min<int>(cliprect.max_y, 383);
			clip_first = max_y;
			clip_last = min_y;
		}

		int zoomx, zoomy;
		if(!(source[0] & 0x2000))
			zoomx = zoomy = source[1] & 0xff;
		else {
			zoomx = source[1] >> 8;
			zoomy = source[1] & 0xff;
		}
		if(!zoomx)
			zoomx = 0x3f;
		if(!zoomy)
			zoomy = 0x3f;

		zoomx++;
		zoomy++;

		x = source[5] & 0xfff;
		flipx = source[5] & 0x8000;
		if(x & 0x800)
			x -= 0x1000;
		sx = 1 << ((source[5] & 0x7000) >> 12);

		x -= 8;

		y = source[4] & 0xfff;
		if(y & 0x800)
			y -= 0x1000;
		flipy = source[4] & 0x8000;
		sy = 1 << ((source[4] & 0x7000) >> 12);

		uint16_t *const pix = &sprite_ram[(source[3] & 0x3fff) * 0x8];
		uint8_t pm[16];
		for(int px=0; px<8; px++) {
			int c;
			c              = pix[px] >> 8;
			pm[px*2]       = pmt[c>>6];
			if(c>1)
				c |= 0x1000;
			colors[px*2]   = c;

			c              = pix[px] & 0xff;
			pm[px*2+1]     = pmt[c>>6];
			if(c>1)
				c |= 0x1000;
			colors[px*2+1] = c;
		}

		const uint32_t offset = (source[2] & 0x7fff) * 0x10;



		int xmod = 0x20;
		int ymod = 0x20;
		for(int py=0; py<sy; py++) {
			int xmod1 = xmod;
			int xpos1 = x;
			int ypos1 = y, ymod1 = ymod;
			for(int px=0; px<sx; px++) {
				int xmod2 = xmod1, xpos2 = xpos1;
				const uint32_t addoffset = 0x10*(flipx ? sx-px-1 : px) + 0x10*sx*(flipy ? sy-py-1 : py) + (flipy ? 7*2 : 0);
				uint32_t newoffset = offset + addoffset;

				ymod1 = ymod;
				ypos1 = y;
				for(int zy=0; zy<8; zy++) {
					ymod1 += zoomy;
					while(ymod1 >= 0x40) {
						if(ypos1 >= min_y && ypos1 <= max_y && (ypos1 <= clip_first || ypos1 >= clip_last)) {
							xmod2 = xmod1;
							xpos2 = xpos1;

							for(int zx=0; zx<8; zx++) {
								xmod2 += zoomx;
								while(xmod2 >= 0x40) {
									if(xpos2 >= min_x && xpos2 <= max_x) {
										int zx1 = flipx ? 7-zx : zx;
										uint32_t neweroffset = (newoffset+(zx1>>2))&0x1ffff; // crackdown sometimes attempts to use data past the end of spriteram
										int c = (sprite_ram[neweroffset] >> (((~zx1) & 3) << 2)) & 0xf;
										uint8_t *pri = &priority_bitmap.pix(ypos1, xpos2);
										if(!(*pri & pm[c])) {
											c = colors[c];
											if(c) {
												uint16_t *dst = &bitmap.pix(ypos1, xpos2);
												if(c==1)
													*dst = (*dst) | 0x2000;
												else
													*dst = c;
												*pri = 0xff;
											}
										}
									}
									xmod2 -= 0x40;
									xpos2++;
								}
							}
						}
						ymod1 -= 0x40;
						ypos1++;
					}
					if(flipy)
						newoffset -= 2;
					else
						newoffset += 2;
				}

				xpos1 = xpos2;
				xmod1 = xmod2;
			}
			y    = ypos1;
			ymod = ymod1;
		}
	}
}


