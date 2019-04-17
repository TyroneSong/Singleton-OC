//
//  NSObject+ALSingletionInit.h
//  SingletonDemo
//
//  Created by 宋璞 on 2019/4/17.
//  Copyright © 2019 宋璞. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSObject (ALSingletionInit)

- (void)singletonInit NS_REQUIRES_SUPER;

@end

NS_ASSUME_NONNULL_END
