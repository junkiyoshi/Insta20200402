#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();

	this->font_size = 18;
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", this->font_size, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);
	ofRotateY(ofGetFrameNum());

	vector<glm::vec3> location_list;
	for (int x = -200; x <= 200; x += 200) {

		for (int y = -200; y <= 200; y += 200) {

			for (int z = -200; z <= 200; z += 200) {

				location_list.push_back(glm::vec3(x, y, z));
			}
		}
	}

	vector<string> word_list = {
		"Hip", "Godzilla", "Meat",
		"Sun", "Cake", "Whisky",
		"Microsoft", "Sony", "C#",

		"Happy", "Leg", "Love",
		"Cute", "oF", "Jump",
		"Cat", "Japan", "Game",

		"C++", "FF14", "Walk",
		"Programming", "Dog", "Hentai",
		"PC", "Run", "Sake",
	};

	int sample_count = 50;
	int word_index = 0;
	for (auto& location : location_list) {

		ofPushMatrix();
		ofTranslate(location);

		auto word = word_list[word_index++];
		auto noise_param = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

		auto word_size = this->font.getStringBoundingBox(word, 0, 0);
		vector<ofPath> word_path = this->font.getStringAsPoints(word, true, false);
		for (int word_index = 0; word_index < word_path.size(); word_index++) {

			vector<ofPolyline> outline = word_path[word_index].getOutline();

			ofFill();
			ofSetColor(39);
			ofBeginShape();
			for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

				if (outline_index != 0) { ofNextContour(true); }

				auto vertices = outline[outline_index].getResampledByCount(sample_count).getVertices();
				for (auto& vertex : vertices) {

					auto noise_location = glm::vec3(
						ofMap(ofNoise(noise_param.x, vertex.x * 0.008 - ofGetFrameNum() * 0.02), 0, 1, -100, 100),
						ofMap(ofNoise(noise_param.y, vertex.x * 0.008 - ofGetFrameNum() * 0.02), 0, 1, -100, 100),
						ofMap(ofNoise(noise_param.z, vertex.x * 0.008 - ofGetFrameNum() * 0.01), 0, 1, -100, 100));

					ofVertex(noise_location + vertex - glm::vec3(word_size.getWidth() * 0.5, -word_size.getHeight() * 0.5, 0));
				}
			}
			ofEndShape();
		}

		ofNoFill();
		ofSetColor(139);
		ofDrawBox(200);

		ofPopMatrix();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}