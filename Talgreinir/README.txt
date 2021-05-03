Leiðbeiningar fyrir RU talgreininn
Prufukóði: tal_stream_test.py

1. Setja upp nauðsynlega pakka í Linux með því að framkvæma eftirfarandi skipanir
	sudo apt-get install libportaudio0 libportaudio2 libportaudiocpp0 portaudio19-dev
	sudo pip3 install pyaudio
	(Sjá: https://makersportal.com/blog/2018/8/23/recording-audio-on-the-raspberry-pi-with-python-and-a-usb-microphone)

2. Fara inn á https://tal.ru.is/stjori/ og skrá sig inn með t.d. Gmail reikningi.
	Búa til lykil, sem lítur svona út
	ak_BAj1dO47QmyPvkDKZ4GBq628OpALW9weW1nXaEN7oj5geYzrdRJV1lM0bmDlK5Lr

3. Setja lykil sem breytu í Linux
	export TALA_API_KEY=ak_BAj1dO47QmyPvkDKZ4GBq628OpALW9weW1nXaEN7oj5geYzrdRJV1lM0bmDlK5Lr
	Þessi breyta tapast þegar Linux er endurræst en það er hægt að festa hana inni með því að
	bæta ofangreindri skipun neðst í skrána ~/.profile -> nano ~/.profile.

4. Opna python koðann
	python3 tal_stream_test.py /bla  (Ath: /bla þarf að vera eins og er, gerir samt ekki neitt)
	Ef það kemur röð af Alsa villum en það þarf ekki endilega að koma að sök.

5. Tala í hljóðnema og þá ætti setningin að birtast fyrst og svo fyrsta orðið.
	----------TRANSCRIPT----------
	hvað segirðu í fréttum

	----------WORD----------
	hvað
	




