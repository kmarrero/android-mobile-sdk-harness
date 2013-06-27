/*
 * SearchExample.h
 *
 *  Created on: Jun 18, 2013
 *      Author: kimbleoperations
 */

#ifndef SEARCHEXAMPLE_H_
#define SEARCHEXAMPLE_H_

#include "IExample.h"
#include "SearchService.h"
#include "NewGlobeCamera.h"
#include "IPoiSearchCallback.h"

namespace Examples
{
    class SearchExample : public IExample, public Eegeo::Search::Service::IPoiSearchCallback
    {
    private:
        Eegeo::Search::Service::SearchService& searchService;
        Eegeo::Camera::NewGlobeCamera& cameraController;

    public:
        SearchExample(Eegeo::Search::Service::SearchService& searchService,
                      Eegeo::Camera::NewGlobeCamera& cameraController);

        void Start();
        void Update() {}
        void Draw() {}
        void Suspend() {}

        void HandlePoiSearchResults(const Eegeo::Search::Service::PoiSearchResult& result);
    };
}

#endif /* SEARCHEXAMPLE_H_ */
